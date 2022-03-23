using namespace std;
#include <vector>
#include <string>

#ifdef ESP32
#include <M5Stack.h>
#define LGFX_M5STACK
#include <LovyanGFX.hpp>
#endif

#ifndef CHRSCREEN_H
#define CHRSCREEN_H

typedef struct {
  wchar_t w;
  uint16_t fg;
  uint16_t bg;
  bool dirty;
} Chr;

class ChrScreen {
  const int fontScale = 1;
  const int fontPointH = 12;
  const int fontPointW = 6;
  int screenWidth = 320;

  public:
  vector<vector<Chr>> lines;
  int cursorCol = 0;
  int cursorLine = 0;
  void init(int w, int h);
#ifdef ESP32
  void draw(LGFX lgfx);
#endif
  void putString(int x, int y, wchar_t* w, int16_t fg, int16_t bg);
  void putString(int x, int y, char* w, int16_t fg, int16_t bg);
  void putChar(int x, int y, wchar_t w, int16_t fg, int16_t bg);
  void clearLine(int y, int16_t fg, int16_t bg);
  wchar_t getChar(int x, int y);
  int getMaxLine();
};

#endif
