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
#ifdef ESP32
#include <luashell.h>
#endif
#include <chrscreen.h>
#include <util.h>

#ifndef SHELL_H
#define SHELL_H
class Shell: public Task{
  public:
  int x = 0;
  int y = 0;
  const int16_t TFT_WHITE = 0xffff;
  const int16_t TFT_BLACK = 0x0000;

  vector<wchar_t> rawInputs;
  vector<wchar_t> ::iterator rawInputsItr;
  //KanjiEditor* editor;
#ifdef ESP32
  LuaShell* luaShell;
#endif

  virtual void init();
  virtual bool onkeydown(char key, char c, bool ctrl);
  virtual void backSpace();
  virtual void right();
  virtual void left();
  virtual void up();
  virtual void down();
  virtual void enter();
  virtual bool onChar(wchar_t c);
  virtual void nextLine();
};
#endif


