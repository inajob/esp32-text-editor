#include <chrscreen.h>

#ifndef LUA_EXEC_H
#define LUA_EXEC_H

extern "C" {
#include "lua.h"

#include "lualib.h"
#include "lauxlib.h"
}

class LuaEngine{
  public:
  lua_State* L;
  const int16_t TFT_WHITE = 0xffff;
  const int16_t TFT_BLACK = 0x0000;

  ChrScreen* chrScreen;
  bool isTerminate;
  void init(ChrScreen* cs);
  void eval(char* utf8LuaString);
  void keydown(char key, char c, bool ctrl); // TODO
  void onChar(char* utf8char); // TODO
  static int l_putString(lua_State* L);
  static int l_exit(lua_State* L);
};

#endif
