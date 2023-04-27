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
  const char *getF(lua_State *L, void *ud, size_t *size){
    struct LoadF *lf = (struct LoadF *)ud;
    (void)L; /* not used */
    char* ret = NULL;

    if(!lf->f.available()){
      *size = 0;
      return NULL;
    }

    lf->f.readStringUntil('\n').toCharArray(lf->buf, MAX_CHAR);
    ret = lf->buf;
    int len = strnlen(ret, MAX_CHAR);
    ret[len] = '\n'; // todo n, n+1 > MAX_CHAR ?
    ret[len + 1] = 0;

    *size = len + 1;
    Serial.print("");
    Serial.println(ret);
    Serial.println(*size);
    return ret;
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
  lua_pushcclosure(L, l_clearLine, 1);
  lua_setglobal(L, "clearline");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_setCursor, 1);
  lua_setglobal(L, "setcursor");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_getMaxLine, 1);
  lua_setglobal(L, "getmaxline");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_getCharWidth, 1);
  lua_setglobal(L, "getcharwidth");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_getScreenWidth, 1);
  lua_setglobal(L, "getscreenwidth");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_setColor, 1);
  lua_setglobal(L, "setcolor");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_debug, 1);
  lua_setglobal(L, "debug");

  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, l_exit, 1);
  lua_setglobal(L, "exit");

  // TODO: load main.lua
  String fileName = "/main.lua";
  File fp = SPIFFS.open(fileName, FILE_READ);
  struct LoadF lf;
  lf.f = fp;
  char cFileName[32];
  bool runError = false;
  fileName.toCharArray(cFileName, 32);
  if(lua_load(L, getF, &lf, cFileName, NULL)){
    Serial.printf("error? %s\n", lua_tostring(L, -1));
    runError = true;
    //errorString = lua_tostring(L, -1);
  }
  fp.close();

  if(runError == false){
    if(lua_pcall(L, 0, 0,0)){
      Serial.printf("init error? %s\n", lua_tostring(L, -1));
      runError = true;
      //errorString = lua_tostring(L, -1);
    }
  }
}
void LuaEngine::eval(char* utf8LuaString){
  int result = luaL_loadstring(L, utf8LuaString);
  if(result != LUA_OK){
    Serial.println("lua error");
    Serial.printf("error? %s\n", lua_tostring(L, -1));
    char* err = (char*)lua_tostring(L, -1);
    chrScreen->setCursor(0,0);
    chrScreen->putString(err, TFT_WHITE, TFT_RED);
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
  lua_getglobal(L, "keydown");
  lua_pushnumber(L, key);
  char s[2];
  s[0] = c;
  s[1] = 0;
  lua_pushstring(L, s);
  lua_pushboolean(L, ctrl);
  if(lua_pcall(L, 3, 0, 0)){
     Serial.printf("run error? %s\n", lua_tostring(L, -1));
     chrScreen->setCursor(0,0);
     chrScreen->putString((char*)lua_tostring(L, -1), TFT_WHITE, TFT_RED);
   }
}
void LuaEngine::onChar(char* utf8char){
  lua_getglobal(L, "onChar");
  lua_pushstring(L, utf8char);
  if(lua_pcall(L, 1, 0, 0)){
     Serial.printf("run error? %s\n", lua_tostring(L, -1));
     chrScreen->setCursor(0,0);
     chrScreen->putString((char*)lua_tostring(L, -1), TFT_WHITE, TFT_RED);
  }
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
  self->chrScreen->putString(utf16, self->fgColor, self->bgColor);
  return 0;
}
int LuaEngine::l_clearLine(lua_State* L){
  LuaEngine* self = (LuaEngine*)lua_touserdata(L, lua_upvalueindex(1));
  const int row = lua_tointeger(L, 1);

  self->chrScreen->clearLine(row, self->fgColor, self->bgColor);
  return 0;
}

int LuaEngine::l_setCursor(lua_State* L){
  LuaEngine* self = (LuaEngine*)lua_touserdata(L, lua_upvalueindex(1));
  const int col = lua_tointeger(L, 1);
  const int line = lua_tointeger(L, 2);

  self->chrScreen->setCursor(col, line);
  return 0;
}
int LuaEngine::l_getMaxLine(lua_State* L){
  LuaEngine* self = (LuaEngine*)lua_touserdata(L, lua_upvalueindex(1));

  int line = self->chrScreen->getMaxLine();
  lua_pushinteger(L, (lua_Integer)line);
  return 1;
}
int LuaEngine::l_getCharWidth(lua_State* L){
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

  int width = self->chrScreen->getCharSize(utf16[0]);
  lua_pushinteger(L, (lua_Integer)width);
  return 1;
}
int LuaEngine::l_getScreenWidth(lua_State* L){
  LuaEngine* self = (LuaEngine*)lua_touserdata(L, lua_upvalueindex(1));

  int width = 320; // TODO
  lua_pushinteger(L, (lua_Integer)width);
  return 1;
}

int LuaEngine::l_setColor(lua_State* L){
  LuaEngine* self = (LuaEngine*)lua_touserdata(L, lua_upvalueindex(1));
  int r,g,b;
  r = lua_tointeger(L, 1);
  g = lua_tointeger(L, 2);
  b = lua_tointeger(L, 3);
  int r2,g2,b2;
  r2 = lua_tointeger(L, 4);
  g2 = lua_tointeger(L, 5);
  b2 = lua_tointeger(L, 6);

  self->fgColor = rgb24to16(r, g, b);
  self->bgColor = rgb24to16(r2, g2, b2);
  return 0;
}
int LuaEngine::l_debug(lua_State* L){
  LuaEngine* self = (LuaEngine*)lua_touserdata(L, lua_upvalueindex(1));
  const char* text = lua_tostring(L, 1);
  Serial.println(text);
  return 0;
}

int LuaEngine::l_exit(lua_State* L){
  LuaEngine* self = (LuaEngine*)lua_touserdata(L, lua_upvalueindex(1));
  self->isTerminate = true;
  return 0;
}


