#include <luashell.h>

// to use utf16CharToUtf8
#include <fep.h>

void LuaShell::init(){
  rawInputsItr = rawInputs.begin();
  for(int i = 0; i < chrScreen->getMaxLine(); i ++){
    chrScreen->clearLine(i, TFT_WHITE, TFT_BLACK);
  }
  lua.init(chrScreen);
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
