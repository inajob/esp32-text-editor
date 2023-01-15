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

#include <chrscreen.h>

#ifndef TASK_H
#define TASK_H
class Task{
  public:
  ChrScreen* chrScreen;
  Task* fep = NULL;
  Task* nextTask = NULL;
  bool isTerminate = false;
  virtual void init();
  virtual void setChrScreen(ChrScreen* chrScreen);
  virtual void setFep(Task* fep);
  virtual bool onkeydown(char key, char c, bool ctrl);
  virtual bool onChar(wchar_t c);
  virtual void draw();
  virtual void setNextTask(Task* t);
  virtual void terminate();
};
#endif
