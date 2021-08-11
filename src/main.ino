#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <SPIFFS.h>

#include <M5Stack.h>
#include <efontEnableJa.h>
#include <efontFontData.h>

#define LGFX_M5STACK
#include <LovyanGFX.hpp>

#include <editor.h>

static LGFX lcd;
const int fontSize = 1;
KanjiEditor editor;

void draw(){
  lcd.clear(BLACK);
  //M5.Lcd.fillRect(0, (line - lines.begin()) * 16, 320, 16, BLACK);
  lcd.setCursor(0,0);

  lcd.setTextColor(0xFFFFFFU);

  // == TODO: move to lib ==

  // draw decided characters
  vector<vector<wchar_t>>::iterator itr;
  vector<wchar_t>::iterator itr2;
  int x = 0 ,y = -16*fontSize;
  for(itr = editor.lines.begin(); itr != editor.lines.end(); itr ++){
    x = 0;
    y += 16*fontSize;
    for(itr2 = itr->begin(); itr2 != itr->end(); itr2 ++){
      char utf8[4];
      utf16CharToUtf8(*itr2, utf8);
      lcd.drawString(utf8, x, y);
      if(isAscii(*itr2)){
        x += 8*fontSize;
      }else{
        x += 16*fontSize;
      }
    }
  }
  // draw un-decided characters
  lcd.setTextColor(0x000000U, 0xFFFFFFU);
  bool hasRawInputs = false;
  for(itr2 = editor.rawInputs.begin(); itr2 != editor.rawInputs.end(); itr2 ++){
    char utf8[4];
    utf16CharToUtf8(*itr2, utf8);
    lcd.drawString(utf8, x, y);
    x += 16*fontSize;
    hasRawInputs = true;
  }
  if(!hasRawInputs){
    if(editor.shiin1 != 0){
      lcd.drawChar((char)editor.shiin1, x, y);
      x += 16*fontSize;
    }
    if(editor.shiin2 != 0){
      lcd.drawChar((char)editor.shiin2, x, y);
      x += 16*fontSize;
    }
  }

  // draw cursor
  int cursorX = 0;
  for(itr2 = editor.line->begin(); itr2 != editor.colItr; itr2 ++){
    if(isAscii(*itr2)){
      cursorX += 8*fontSize;
    }else{
      cursorX += 16*fontSize;
    }
  }
  int cursorWidth = 16;
  if(editor.colItr != editor.line->begin()){
    if(isAscii(*(editor.colItr))){
      cursorWidth = 8;
    }
  }else{
    if(editor.line->end() - editor.line->begin() > 0){
      if(isAscii(*(editor.colItr))){
        cursorWidth = 8;
      }
    }
  }
  lcd.drawRect(cursorX, (editor.line - editor.lines.begin()) * 16 * fontSize, cursorWidth, 16*fontSize, WHITE);

  Serial.print("xy:");
  Serial.print(editor.colItr - editor.line->begin());
  Serial.print(",");
  Serial.println(editor.line - editor.lines.begin());

  // draw kanji list
  if(editor.kanjiMode == KanjiMode::HENKAN){
    lcd.setCursor(0, 240 - 2 * 16 * fontSize);
    for(vector<string>:: iterator kanji = editor.kanjiList.begin(); kanji != editor.kanjiList.end(); kanji ++){
      if(kanji == editor.kanjiListItr){
        lcd.setTextColor(0x000000U, 0xFFFFFFU);
      }else{
        lcd.setTextColor(0xFFFFFFU);
      }
      // kanji is utf8
      const char* k = kanji->c_str();
      lcd.print(k);
      lcd.print(' ');
    }
  }
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
    if(ctrl){
      if(tolower(c) == 'j'){
        editor.kanjiMode = KanjiMode::ROME;
      }
    }else{
      if(editor.kanjiMode == KanjiMode::DIRECT){
        editor.onChar(c);
      }else{
        if(isalpha(c) && c == toupper(c)){
          if(editor.kanjiMode == KanjiMode::ROME || editor.kanjiMode == KanjiMode::KATA){
            editor.setStartKanjiMode();
            editor.onCharRoma(tolower(c));
          }else if(editor.kanjiMode == KanjiMode::KANJI){
            editor.onCharRoma(tolower(c)); // only in shiin
            editor.rawInputsItr = editor.rawInputs.insert(editor.rawInputsItr, tolower(c));
            editor.rawInputsItr ++;
            editor.kanjiHenkan();
          }else if(editor.kanjiMode == KanjiMode::HENKAN){
            editor.kanjiDecide();
            editor.setStartKanjiMode();
            editor.onCharRoma(tolower(c));
          }
        }else{
          editor.onCharRoma(c);
        }
      }
      draw();
    }
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

  editor.onCharRoma(c);
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
  lcd.setFont(&fonts::efont);
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

  editor.initEditor();

  lcd.clear(BLACK);
  draw();
}

void loop()
{
  Usb.Task();
}
