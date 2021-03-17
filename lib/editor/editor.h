using namespace std;
#include <vector>
#include <cstdlib> // for native test
#include <stdint.h> // for native test

#ifndef EDITOR_H
#define EDITOR_H

extern vector<vector<wchar_t>> lines;

extern vector<vector<wchar_t>> ::iterator line;
extern vector<wchar_t> ::iterator colItr;
extern uint8_t shiin1;

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

#endif
