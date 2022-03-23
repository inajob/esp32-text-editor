using namespace std;
#include <vector>

#ifndef UTIL_H
#define UTIL_H

void to_wchar(vector<wchar_t> line, wchar_t* w, int n);
void to_char(vector<wchar_t> line, char* c, int n);
vector<vector<wchar_t>> split(vector<wchar_t> line);
#endif
