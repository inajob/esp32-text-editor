#include <luashell.h>

// to use utf16CharToUtf8
#include <fep.h>

void LuaShell::init(){
  rawInputsItr = rawInputs.begin();
  for(int i = 0; i < chrScreen->getMaxLine(); i ++){
    chrScreen->clearLine(i, TFT_WHITE, TFT_BLACK);
  }
  chrScreen->setCursor(0, 0);
  chrScreen->putChar(L'>', TFT_WHITE, TFT_BLACK);
  chrScreen->putChar(L' ', TFT_WHITE, TFT_BLACK);

  lua.init(chrScreen);
}
bool LuaShell::onkeydown(char key, char c, bool ctrl){
  if(fep){
    bool through = fep->onkeydown(key, c, ctrl);
    if(!through){
      return false;
    }
  }
  switch(key){
    case 0x2A: // BS
      backSpace();
    break;
    case 0x50: // <-
      left();
    break;
    case 0x4F: // ->
      right();
    break;
    case 0x52: // ^
      up();
    break;
    case 0x51: // v
      down();
    break;
    default:
      if(c == '\r'){
        enter();
      }else{
        onChar(c);
      }
  }
  return true;
}
void LuaShell::backSpace(){
  if(rawInputs.size() == 0){
    return;
  }
  rawInputs.pop_back();
  chrScreen->back();
  chrScreen->putChar(0, TFT_WHITE, TFT_BLACK);
  chrScreen->putChar(0, TFT_WHITE, TFT_BLACK);
  chrScreen->back();
  chrScreen->back();
}
void LuaShell::right(){
}
void LuaShell::left(){
}
void LuaShell::up(){
}
void LuaShell::down(){
}

void LuaShell::enter(){
  nextLine();
  if(rawInputs.size() == 0){
    chrScreen->putChar(L'>', TFT_WHITE, TFT_BLACK);
    chrScreen->putChar(L' ', TFT_WHITE, TFT_BLACK);

    return;
  }
  int count = 0;
  char utf8LuaString[1024];
  for(vector<wchar_t>::iterator itr = rawInputs.begin(); itr != rawInputs.end(); itr ++){
    char utf8[4];
    uint8_t len = utf16CharToUtf8(*itr, utf8);
    for(int i = 0; i < len; i ++){
      utf8LuaString[count] = utf8[i];
      count ++;
    }
  }
  utf8LuaString[count] = 0;
  lua.eval(utf8LuaString);

  x = 0;
  rawInputs.clear();
  rawInputsItr = rawInputs.begin();

  chrScreen->putChar(L'>', TFT_WHITE, TFT_BLACK);
  chrScreen->putChar(L' ', TFT_WHITE, TFT_BLACK);
}
bool LuaShell::onChar(wchar_t c){
  chrScreen->putChar(c, TFT_WHITE, TFT_BLACK);
  rawInputs.push_back(c);
  return true;
}
void LuaShell::nextLine(){
  chrScreen->nextLine();
}
