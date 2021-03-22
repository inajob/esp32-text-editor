using namespace std;
#include <vector>
#include <string>
#include <cstdlib> // for native test
#include <stdint.h> // for native test
#include <dicttool.h>

#ifndef EDITOR_H
#define EDITOR_H

extern vector<vector<wchar_t>> lines;

extern vector<vector<wchar_t>> ::iterator line;
extern vector<wchar_t> ::iterator colItr;
extern uint8_t shiin1;
extern uint8_t shiin2;
extern vector<wchar_t> rawInputs;
extern vector<wchar_t> ::iterator rawInputsItr;
extern vector<string> kanjiList;

enum class KanjiMode {
  DIRECT = 0,
  KANJI,
  HENKAN
};

extern KanjiMode kanjiMode;

uint8_t utf16CharToUtf8(wchar_t utf16, char* utf8);
size_t utf8CharToUtf16(char* utf8, wchar_t* utf16);

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

#endif
