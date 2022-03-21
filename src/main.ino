#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <SPIFFS.h>

#include <M5Stack.h>
//#include <efontEnableJa.h>
//#include <efontFontData.h>

#define LGFX_M5STACK
#include <LovyanGFX.hpp>

#include <editor.h>
#include <chrscreen.h>

LGFX lcd;
const int fontSize = 1;
const int fontPointH = 12;
const int fontPointW = 6;
KanjiEditor editor;
ChrScreen chrScreen;

void draw(){
  // == TODO: move to lib ==

  int cursorX = 0;
  int cursorY = 0;

  // draw decided characters
  vector<vector<wchar_t>>::iterator itr;
  vector<wchar_t>::iterator itr2;
  int x = 0, y = 0;
  for(itr = editor.lines.begin(); itr != editor.lines.end(); itr ++){
    chrScreen.clearLine(y, TFT_WHITE, TFT_BLACK);
    chrScreen.putChar(0, y, (wchar_t)('0' + y/10), TFT_WHITE, TFT_BLACK);
    chrScreen.putChar(1, y, (wchar_t)('0' + y%10), TFT_WHITE, TFT_BLACK);
    x += 3;
    for(itr2 = itr->begin(); itr2 != itr->end(); itr2 ++){
      if(itr2 == editor.colItr){
        cursorX = x;
        cursorY = y;
        chrScreen.putChar(x, y, *itr2, TFT_BLACK, TFT_WHITE);
      }else{
        chrScreen.putChar(x, y, *itr2, TFT_WHITE, TFT_BLACK);
      }
      x ++;
    }
    if(editor.line == itr && itr->end() == editor.colItr){
      chrScreen.putChar(x, y, 0, TFT_BLACK, TFT_WHITE);
      cursorX = x;
      cursorY = y;
    }
    x = 0;
    y ++;
    if(y == chrScreen.getMaxLine() - 2){
      break;
    }
  }
  chrScreen.clearLine(y, TFT_WHITE, TFT_BLACK);

  // draw un-decided characters
  x = cursorX;
  y = cursorY;

  bool hasRawInputs = false;
  for(itr2 = editor.rawInputs.begin(); itr2 != editor.rawInputs.end(); itr2 ++){
    char utf8[4];
    chrScreen.putChar(x, y, *itr2, TFT_BLACK, TFT_WHITE);
    x ++;
    hasRawInputs = true;
  }
  if(!hasRawInputs){
    if(editor.shiin1 != 0){
      chrScreen.putChar(x, y,(wchar_t)editor.shiin1, TFT_BLACK, TFT_WHITE);
      x ++;
    }
    if(editor.shiin2 != 0){
      chrScreen.putChar(x, y, (wchar_t)editor.shiin2, TFT_BLACK, TFT_WHITE);
      x ++;
    }
  }

  // mode line
  switch(editor.kanjiMode){
    case KanjiMode::DIRECT: chrScreen.putString(0, chrScreen.getMaxLine() - 1, L"[A]", TFT_BLACK, TFT_WHITE); break;
    case KanjiMode::KATA:   chrScreen.putString(0, chrScreen.getMaxLine() - 1, L"[ア]", TFT_BLACK, TFT_WHITE); break;
    case KanjiMode::ROME:   chrScreen.putString(0, chrScreen.getMaxLine() - 1, L"[あ]", TFT_BLACK, TFT_WHITE); break;
    case KanjiMode::KANJI:  chrScreen.putString(0, chrScreen.getMaxLine() - 1, L"[漢]", TFT_BLACK, TFT_WHITE); break;
    case KanjiMode::HENKAN: break;
  }

  x = 0;
  y = chrScreen.getMaxLine() - 2;
  chrScreen.clearLine(y, TFT_WHITE, TFT_BLACK);
  if(editor.kanjiMode == KanjiMode::HENKAN){
    for(vector<string>:: iterator kanji = editor.kanjiList.begin(); kanji != editor.kanjiList.end(); kanji ++){
      int16_t fg = TFT_WHITE;
      int16_t bg = TFT_BLACK;
      if(kanji == editor.kanjiListItr){
        fg = TFT_BLACK;
        bg = TFT_WHITE;
      }
      // kanji is utf8
      const char* k = kanji->c_str();
      wchar_t w;
      while(*k != 0){
        size_t n = utf8CharToUtf16((char*)k, &w);
        //lcd.print(k);
        chrScreen.putChar(x, y, w, fg, bg);
        x ++;
        k += n;
      }
      chrScreen.putChar(x, y, L' ', fg, bg);
      x ++;
      // TODO: overflow x
    }
  }

  chrScreen.draw(lcd);
}

class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown	(uint8_t mod, uint8_t key);
    void OnKeyUp	(uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t c);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
  Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
  Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

  Serial.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< ");

  Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
  Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
  Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  Serial.print("DN ");
  PrintKey(mod, key);
  uint8_t c = OemToAscii(mod, key);

  // BS key == 0x2A
  if(key == 0x2A){
    editor.backSpace();
    draw();
    return;
  }
  // <- 0x50
  if(key == 0x50){
    editor.left();
    draw();
    return;
  }
  // -> 0x4F
  if(key == 0x4F){
    editor.right();
    draw();
    return;
  }
  // ^ 0x52
  if(key == 0x52){
    editor.up();
    draw();
    return;
  }
  // v 0x51
  if(key == 0x51){
    editor.down();
    draw();
    return;
  }

  if (c == '\r'){
    editor.enter();
    draw();
    return;
  }
  if (c){
    //uint8_t shift = (mod & 0x22);
    uint8_t ctrl = (mod & 0x11);
    //OnKeyPressed(c); // no use now
    editor.onCharRoma(c, ctrl);
    draw();
  }
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    Serial.println("LeftCtrl changed");
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    Serial.println("LeftShift changed");
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    Serial.println("LeftAlt changed");
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    Serial.println("LeftGUI changed");
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    Serial.println("RightCtrl changed");
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    Serial.println("RightShift changed");
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    Serial.println("RightAlt changed");
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    Serial.println("RightGUI changed");
  }

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  Serial.print("UP ");
  PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t c)
{
 /* no use */
  Serial.print("ASCII: ");
  Serial.println((char)c);
  //M5.Lcd.print((char)c);

  //editor.onCharRoma(c);
  //onChar(c);
  draw();
};

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup()
{
  Serial.begin( 115200 );
  //M5.begin();
  SPIFFS.begin();
  lcd.init();
  lcd.setTextSize(fontSize, fontSize);
  lcd.setTextColor(0xFFFFFFU);
  //lcd.setFont(&fonts::efont);
  lcd.setFont(&fonts::efontJA_12);
  //M5.Lcd.setTextSize(2);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  HidKeyboard.SetReportParser(0, &Prs);
  lcd.println("Start");

  chrScreen.init(320, 240);
  editor.initEditor();

  lcd.clear(BLACK);
  draw();
}

void loop()
{
  Usb.Task();
}
