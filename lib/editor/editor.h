using namespace std;
#include <vector>
#include <string>
#include <cstdlib> // for native test
#include <stdint.h> // for native test
#include <dicttool.h>

#ifndef EDITOR_H
#define EDITOR_H

enum class KanjiMode {
  DIRECT = 0,
  KANJI,
  HENKAN
};

uint8_t utf16CharToUtf8(wchar_t utf16, char* utf8);
size_t utf8CharToUtf16(char* utf8, wchar_t* utf16);

class Editor{
  public:
  vector<vector<wchar_t>> lines;
  vector<vector<wchar_t>> ::iterator line;
  vector<wchar_t> ::iterator colItr;

  KanjiMode kanjiMode = KanjiMode::DIRECT; // TODO: move
  vector<wchar_t> rawInputs; // TODO: move
  vector<wchar_t> ::iterator rawInputsItr; //TODO: move

  virtual void initEditor();
  virtual void backSpace();
  virtual void right();
  virtual void left();
  virtual void up();
  virtual void down();
  virtual void enter();
  virtual void onChar(wchar_t c);
};

class KanjiEditor: public Editor{
  public:
  uint8_t shiin1 = 0;
  uint8_t shiin2 = 0;
  vector<string> kanjiList;

  void initEditor();
  void backSpace();
  void right();
  void left();
  void up();
  void down();
  void enter();
  void onChar(wchar_t c);
  void onCharRoma(uint8_t c);
  void onBoin(uint8_t c);
  void setStartKanjiMode();
  void kanjiHenkan();
  void kanjiDecide();
};
#endif
