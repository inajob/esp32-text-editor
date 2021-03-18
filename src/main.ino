#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <M5Stack.h>
#include <efontEnableJa.h>
#include <efontFontData.h>

#define LGFX_M5STACK
#include <LovyanGFX.hpp>

#include <editor.h>

static LGFX lcd;
const int fontSize = 3;

void draw(){
  lcd.clear(BLACK);
  //M5.Lcd.fillRect(0, (line - lines.begin()) * 16, 320, 16, BLACK);
  lcd.setCursor(0,0);

  // == TODO: move to lib ==
  vector<vector<wchar_t>>::iterator itr;
  vector<wchar_t>::iterator itr2;
  int x = 0 ,y = 0;
  for(itr = lines.begin(); itr != lines.end(); itr ++){
    x = 0;
    for(itr2 = itr->begin(); itr2 != itr->end(); itr2 ++){
      char u8Ch[4];
      // convert utf16 -> utf8
      uint16_t u16Ch = (int16_t)(*itr2);
      if (u16Ch < 128) {
        u8Ch[0] = char(u16Ch);
        u8Ch[1] = 0;
        u8Ch[2] = 0;
      } else if (u16Ch < 2048) {
        u8Ch[0] = 0xC0 | char(u16Ch >> 6);
        u8Ch[1] = 0x80 | (char(u16Ch) & 0x3F);
        u8Ch[2] = 0;
      } else if (u16Ch < 65536) {
        u8Ch[0] = 0xE0 | char(u16Ch >> 12);
        u8Ch[1] = 0x80 | (char(u16Ch >> 6) & 0x3F);
        u8Ch[2] = 0x80 | (char(u16Ch) & 0x3F);
        u8Ch[3] = 0;
      }

      lcd.drawString(u8Ch, x, y);
      x += 16*fontSize;
    }
    y += 16*fontSize;
  }
  int dx = 0;
  if(shiin1 != 0){
    lcd.drawChar((char)shiin1, ((colItr - line->begin()) + dx)*16*fontSize, (line - lines.begin()) * 16 * fontSize);
    dx ++;
  }
  if(shiin2 != 0){
    lcd.drawChar((char)shiin2, ((colItr - line->begin()) + dx)*16*fontSize, (line - lines.begin()) * 16 * fontSize);
    dx ++;
  }

  lcd.drawRect((colItr - line->begin())*16*fontSize, (line - lines.begin()) * 16 * fontSize, 16*fontSize, 16*fontSize, WHITE);
  Serial.print("xy:");
  Serial.print(colItr - line->begin());
  Serial.print(",");
  Serial.println(line - lines.begin());
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
    backSpace();
    draw();
    return;
  }
  // <- 0x50
  if(key == 0x50){
    left();
    draw();
    return;
  }
  // -> 0x4F
  if(key == 0x4F){
    right();
    draw();
    return;
  }
  // ^ 0x52
  if(key == 0x52){
    up();
    draw();
    return;
  }
  // v 0x51
  if(key == 0x51){
    down();
    draw();
    return;
  }

  if (c == '\r'){
    enter();
    draw();
    return;
  }
  if (c)
    OnKeyPressed(c);
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
  Serial.print("ASCII: ");
  Serial.println((char)c);
  //M5.Lcd.print((char)c);

  onCharRoma(c);
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

  initEditor();

  lcd.clear(BLACK);
  draw();
}

void loop()
{
  Usb.Task();
}
