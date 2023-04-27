#include <chrscreen.h>

#ifndef LUA_EXEC_H
#define LUA_EXEC_H

extern "C" {
#include "lua.h"

#include "lualib.h"
#include "lauxlib.h"
}

#define MAX_CHAR 256

struct LoadF{
  File f;
  char buf[MAX_CHAR];
};

inline uint16_t rgb24to16(uint8_t r, uint8_t g, uint8_t b) {
  uint16_t tmp = ((r>>3) << 11) | ((g>>2) << 5) | (b>>3);
  return tmp; //(tmp >> 8) | (tmp << 8);
}

class LuaEngine{
  public:
  lua_State* L;
  const int16_t TFT_WHITE = 0xffff;
  const int16_t TFT_BLACK = 0x0000;
  const int16_t TFT_RED = 0xF800;
  int16_t fgColor = 0xffff;
  int16_t bgColor = 0x0000;

  ChrScreen* chrScreen;
  bool isTerminate;
  void init(ChrScreen* cs);
  void eval(char* utf8LuaString);
  void keydown(char key, char c, bool ctrl); // TODO
  void onChar(char* utf8char); // TODO
  static int l_putString(lua_State* L);
  static int l_setCursor(lua_State* L);
  static int l_getMaxLine(lua_State* L);
  static int l_getCharWidth(lua_State* L);
  static int l_getScreenWidth(lua_State* L);
  static int l_setColor(lua_State* L);
  static int l_exit(lua_State* L);
};

#endif
