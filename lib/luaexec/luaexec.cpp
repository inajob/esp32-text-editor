#include <luaexec.h>

#include <fep.h>
// to use utf16CharToUtf8

#include <fep.h>


#include <Arduino.h>

extern "C" {
int gprint(char* s){
  Serial.println(s);
  return 0;
}
}

void LuaEngine::init(ChrScreen* cs){
  isTerminate = false;
  chrScreen = cs;

  L = luaL_newstate();
  luaL_openlibs(L);

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_putString, 1);
  lua_setglobal(L, "putstring");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_exit, 1);
  lua_setglobal(L, "exit");
}
void LuaEngine::eval(char* utf8LuaString){
  int result = luaL_loadstring(L, utf8LuaString);
  if(result != LUA_OK){
    Serial.println("lua error");
    Serial.printf("error? %s\n", lua_tostring(L, -1));
    char* err = (char*)lua_tostring(L, -1);
    chrScreen->putString(err, TFT_WHITE, TFT_BLACK);
  }else{
    Serial.println("lua ok");

    if(lua_pcall(L,0,0,0) != LUA_OK) {
      Serial.println("lua error");
      Serial.println(lua_tostring(L, -1));
      //lua_close(L);
      //exit(EXIT_FAILURE);
    }
  }
}
void LuaEngine::keydown(char key, char c, bool ctrl){
}
void LuaEngine::onChar(char* utf8char){
}

int LuaEngine::l_putString(lua_State* L){
  LuaEngine* self = (LuaEngine*)lua_touserdata(L, lua_upvalueindex(1));
  const char* text = lua_tostring(L, 1);
  wchar_t utf16[1024];
  wchar_t* p = utf16;
  char* o = (char*)text;
  while(*o != 0){
    size_t n = utf8CharToUtf16(o, p);
    o += n;
    p ++;
  }
  *(p++) = 0; // null terminate

  // text is UTF8
  self->chrScreen->putString(utf16, self->TFT_WHITE, self->TFT_BLACK);
  return 0;
}

int LuaEngine::l_exit(lua_State* L){
  LuaEngine* self = (LuaEngine*)lua_touserdata(L, lua_upvalueindex(1));
  self->isTerminate = true;
  return 0;
}


