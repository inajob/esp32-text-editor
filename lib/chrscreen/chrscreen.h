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
  int cursorCol = 0; // wchar_t positoin
  int cursorColPos = 0; // size position
  int cursorLine = 0;
  void init(int w, int h);
#ifdef ESP32
  void draw(LGFX lgfx);
#endif
  void setCursor(int col, int line);
  void putString(wchar_t* w, int16_t fg, int16_t bg);
  void putString(char* w, int16_t fg, int16_t bg);
  void putChar(wchar_t w, int16_t fg, int16_t bg);
  void back();
  void nextLine();
  void clearLine(int line, int16_t fg, int16_t bg);
  void resetLine(int line);
  wchar_t getChar(int col, int line);
  int getCharSize(wchar_t w);
  int getMaxLine();
  int getMaxColumn();
};

#endif
