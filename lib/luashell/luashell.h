using namespace std;
#include <vector>
#include <string>
#include <cstdlib> // for native test
#include <stdint.h> // for native test

#ifdef ESP32
#include <M5Stack.h>
#define LGFX_M5STACK
#include <LovyanGFX.hpp>
#endif

#include <task.h>
//#include <editor.h>
#include <chrscreen.h>
#include <util.h>
#include <luaexec.h>

#ifndef LUA_SHELL_H
#define LUA_SHELL_H

class LuaShell: public Task{
  public:
  int x = 0;
  int y = 0;
  lua_State* L;
  const int16_t TFT_WHITE = 0xffff;
  const int16_t TFT_BLACK = 0x0000;

  LuaEngine lua;
  vector<wchar_t> rawInputs;
  vector<wchar_t> ::iterator rawInputsItr;
  //KanjiEditor* editor;

  virtual void init();
  virtual bool onkeydown(char key, char c, bool ctrl);
  virtual bool onChar(wchar_t c);
};
#endif


