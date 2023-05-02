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
#include <chrscreen.h>
#include <dicttool.h>

#ifndef EDITOR_H
#define EDITOR_H

enum class KanjiMode {
  DIRECT = 0,
  KATA,
  ROME,
  KANJI,
  HENKAN
};

bool isAscii(wchar_t utf16);
uint8_t utf16CharToUtf8(wchar_t utf16, char* utf8);
size_t utf8CharToUtf16(char* utf8, wchar_t* utf16);
// TODO: GetUtf8ByteCount

class KanjiFep: public Task{
  public:
  vector<wchar_t> line;
  vector<wchar_t> ::iterator colItr;
  Task* parentTask;

  KanjiMode kanjiMode = KanjiMode::ROME;
  uint8_t shiin1 = 0;
  uint8_t shiin2 = 0;
  bool isBackspace = false;
  const int16_t TFT_WHITE = 0xffff;
  const int16_t TFT_BLACK = 0x0000;

  string dictPath;
  vector<string> kanjiList;
  vector<string> ::iterator kanjiListItr;
  vector<wchar_t> rawInputs;
  vector<wchar_t> ::iterator rawInputsItr;

  // rome to hira table
  static wchar_t table[][5];
  static wchar_t kata_table[][5];

  void init();
  void setParentTask(Task* t);
  bool onkeydown(char key, char c, bool ctrl);
  bool backSpace();
  bool right();
  bool left();
  bool up();
  bool down();
  bool enter();
  bool onChar(wchar_t c);
  wchar_t getKana(int r, int c);
  bool onCharRoma(uint8_t c, bool ctrl = false);
  void onBoin(uint8_t c);
  void setStartKanjiMode();
  void kanjiHenkan();
  void kanjiDecide();
  void nextKanji();
  void prevKanji();
  void draw();
};
#endif
