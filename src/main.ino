#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include <M5Stack.h>

using namespace std;

#include <vector>

vector<vector<uint16_t>> lines;

vector<vector<uint16_t>> ::iterator line;
vector<uint16_t> ::iterator colItr;

void draw(){
  M5.Lcd.clear(BLACK);
  //M5.Lcd.fillRect(0, (line - lines.begin()) * 16, 320, 16, BLACK);
  M5.Lcd.setCursor(0,0);
  vector<vector<uint16_t>>::iterator itr;
  vector<uint16_t>::iterator itr2;
  for(itr = lines.begin(); itr != lines.end(); itr ++){
    for(itr2 = itr->begin(); itr2 != itr->end(); itr2 ++){
      M5.Lcd.print((char)*itr2);
    }
    M5.Lcd.println();
  }
  M5.Lcd.drawRect((colItr - line->begin())*12, (line - lines.begin()) * 16, 12, 16, WHITE);
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
    if(colItr != line->begin()){
      colItr --;
      colItr = line->erase(colItr);
    }else{
      // marge lines
      vector<uint16_t> ::iterator beginItr = colItr;
      vector<uint16_t> ::iterator endItr = line->end();
      vector<vector<uint16_t>> ::iterator prevLine;

      line --;
      colItr = line->end();
      copy(beginItr, endItr, back_inserter(*line));
      lines.erase(prevLine);
    }
    draw();
    return;
  }
  // <- 0x50
  if(key == 0x50 && colItr != line->begin()){
    colItr --;
    draw();
    return;
  }
  // -> 0x4F
  if(key == 0x4F && colItr != line->end()){
    colItr ++;
    draw();
    return;
  }
  // ^ 0x52
  if(key == 0x52 && line != lines.begin()){
    int pos = colItr - line->begin();

    line --;
    colItr = line->begin();
    for(;pos != 0 && colItr != line->end(); pos--){
      colItr ++;
    }

    draw();
    return;
  }
  // v 0x51
  if(key == 0x51 && line + 1 != lines.end()){
    int pos = colItr - line->begin();

    line ++;
    colItr = line->begin();
    for(;pos != 0 && colItr != line->end(); pos--){
      colItr ++;
    }


    draw();
    return;
  }

  if (c == '\r'){
    vector<uint16_t> newLine;

    copy(colItr, line->end(), back_inserter(newLine));
    while(colItr != line->end()){
      colItr = line->erase(colItr);
    }

    line ++;
    line = lines.insert(line, newLine);

    colItr = line->begin();
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

  colItr = line->insert(colItr, (uint16_t)c);
  colItr ++;
  draw();
};

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

void setup()
{
  Serial.begin( 115200 );
  M5.begin();
  M5.Lcd.setTextSize(2);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  delay( 200 );

  HidKeyboard.SetReportParser(0, &Prs);
  M5.Lcd.println("Start");

  line = lines.begin(); // lines
  //lines.push_back(vector<uint16_t>());
  line = lines.insert(line, vector<uint16_t>());
  colItr = line->begin(); // line
  M5.Lcd.clear(BLACK);
  draw();
}

void loop()
{
  Usb.Task();
}
