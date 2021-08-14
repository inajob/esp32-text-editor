using namespace std;
#include <vector>
#include <string>
#ifdef ESP32
#include <SPIFFS.h>
#include <FS.h>
#endif
#include <cstdlib> // for native test
#include <stdint.h> // for native test
#include <stdio.h> // for native test

#ifndef DICTTOOL_H
#define DICTTOOL_H

void search(char* target, vector<string>* results, char* path);

#endif
