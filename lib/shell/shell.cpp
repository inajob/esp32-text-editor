#include <shell.h>

void Shell::init(){
  rawInputsItr = rawInputs.begin();
  for(int i = 0; i < chrScreen->getMaxLine(); i ++){
    chrScreen->clearLine(i, TFT_WHITE, TFT_BLACK);
  }
  chrScreen->setCursor(0, 0);
  chrScreen->putChar(L'>', TFT_WHITE, TFT_BLACK);
  chrScreen->putChar(L' ', TFT_WHITE, TFT_BLACK);
}
bool Shell::onkeydown(char key, char c, bool ctrl){
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
void Shell::backSpace(){
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
void Shell::right(){
}
void Shell::left(){
}
void Shell::up(){
}
void Shell::down(){
}
void Shell::enter(){
  nextLine();
  int i = 0;
  wchar_t cmd[256];
  char debug[256];
  if(rawInputs.size() == 0){
    chrScreen->putChar(L'>', TFT_WHITE, TFT_BLACK);
    chrScreen->putChar(L' ', TFT_WHITE, TFT_BLACK);

    return;
  }
  for(vector<wchar_t>::iterator itr = rawInputs.begin(); itr != rawInputs.end(); itr ++){
    //chrScreen->putChar(*itr, TFT_WHITE, TFT_BLACK);
    cmd[i] = *itr;
    i ++;
  }
  cmd[i] = 0;
  vector<vector<wchar_t>> args = split(rawInputs);
  to_wchar(args.at(0), cmd, 256);

  if(wcsncmp(cmd, L"ls", 256) == 0){
#ifdef ESP32
    File root = SD.open("/");
    File f = root.openNextFile();
    while(f){
      chrScreen->putString((char*)f.name(), TFT_WHITE, TFT_BLACK);
      f = root.openNextFile();
      nextLine();
    }
#endif
  }else if(wcsncmp(cmd, L"clear", 256) == 0){
    for(int i = 0; i < chrScreen->getMaxLine(); i ++){
      chrScreen->clearLine(i, TFT_WHITE, TFT_BLACK);
    }
    x = 0;
    y = 0;
    chrScreen->setCursor(0, 0);
  }else if(wcsncmp(cmd, L"lua", 256) == 0){
#ifdef ESP32
    if(args.size() > 1){
      char filename[256];
      to_char(args.at(1), filename, 256);

      luaShell->lua.isSD = true;
      luaShell->lua.fileName = filename;
    }else{
      luaShell->lua.isSD = false;
      luaShell->lua.fileName = "/main.lua";
    }

    setNextTask(luaShell);
    for(int i = 0; i < chrScreen->getMaxLine(); i ++){
      chrScreen->clearLine(i, TFT_WHITE, TFT_BLACK);
    }
    rawInputs.clear();
    luaShell->init();
    return;
#endif
  }/*else if(wcsncmp(cmd, L"edit", 256) == 0){
    if(args.size() > 1){
      to_char(args.at(1), editor->filename, 256); // todo: implement setter
      editor->load();
    }
    setNextTask(editor);
    for(int i = 0; i < chrScreen->getMaxLine(); i ++){
      chrScreen->clearLine(i, TFT_WHITE, TFT_BLACK);
    }
    rawInputs.clear();
    return;
  }*/else if(wcsncmp(cmd, L"rm", 256) == 0){
    if(args.size() > 1){
      char filename[256];
      to_char(args.at(1), filename, 256);
#ifdef ESP32
      SD.remove(filename);
#endif
      sprintf(debug, "%s removed", filename);
      chrScreen->putString(debug, TFT_WHITE, TFT_BLACK);
      nextLine();
    }
  }else{
    chrScreen->putString((char*)L"unknown command", TFT_WHITE, TFT_BLACK);
    nextLine();
  }

  x = 0;
  rawInputs.clear();
  rawInputsItr = rawInputs.begin();

  chrScreen->putChar(L'>', TFT_WHITE, TFT_BLACK);
  chrScreen->putChar(L' ', TFT_WHITE, TFT_BLACK);
}
bool Shell::onChar(wchar_t c){
  chrScreen->putChar(c, TFT_WHITE, TFT_BLACK);
  rawInputs.push_back(c);
  return true;
}
void Shell::nextLine(){
  chrScreen->nextLine();
}
