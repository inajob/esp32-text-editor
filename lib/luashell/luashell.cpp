#include <luashell.h>

// to use utf16CharToUtf8
#include <fep.h>
#include <SD.h>

// Define CS pin for the SD card module
#define SD_MISO     19
#define SD_MOSI     23
#define SD_SCLK     18
#define SD_CS       4
SPIClass sdSPI(VSPI);

void LuaShell::init(){
  rawInputsItr = rawInputs.begin();
  for(int i = 0; i < chrScreen->getMaxLine(); i ++){
    chrScreen->clearLine(i, TFT_WHITE, TFT_BLACK);
  }
  lua.init(chrScreen);
  sdSPI.begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  if(!SD.begin(SD_CS, sdSPI)){
    Serial.println("SD init failed");
  }
  isTerminate = false;
}
bool LuaShell::onkeydown(char key, char c, bool ctrl){
  if(fep){
    bool through = fep->onkeydown(key, c, ctrl);
    if(!through){
      return false;
    }
  }
  lua.keydown(key, c, ctrl);
  isTerminate = lua.isTerminate;
}
bool LuaShell::onChar(wchar_t c){
  char utf8char[4];
  int count = 0;
  uint8_t len = utf16CharToUtf8(c, utf8char);

  lua.onChar(utf8char);
  return true;
}
