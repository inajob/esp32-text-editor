#include <chrscreen.h>
#include <editor.h>

// w means the number of half characters
void ChrScreen::init(int w, int h){
  vector<vector<Chr>> ::iterator line;
  screenWidth = w;

  // create character buffer
  for(int i = 0; i < h/(fontPointH * fontScale); i ++){
    line = lines.begin();
    lines.insert(line, vector<Chr>(w/(fontPointW * fontScale)));
  }
}
#ifdef ESP32
void ChrScreen::draw(LGFX lgfx){
  vector<vector<Chr>>::iterator itr;
  vector<Chr>::iterator itr2;
  int x = 0;
  int y = 0;

  // draw character screen
  int lineNo = 0;
  int colNo = 0;
  for(itr = lines.begin(); itr != lines.end(); itr ++){
    colNo = 0;
    for(itr2 = itr->begin(); itr2 != itr->end(); itr2 ++){
      char utf8[4];
      if(itr2->w == 0){ // null character
        if(itr2->dirty){
          lgfx.fillRect(x, y, fontPointW * fontScale, fontPointH * fontScale, itr2->bg);
          itr2->dirty = false;
        }
        x += fontPointW * fontScale;
      }else{ // not null character
        utf16CharToUtf8(itr2->w, utf8); // TODO: move from editor to util

        if(itr2->dirty){
          if(lineNo == cursorLine && colNo == cursorCol){
            // draw cursor
            lgfx.drawRect(x, y, fontPointW * fontScale, fontPointH * fontScale, itr2->fg);
          }
          // draw char
          lgfx.drawRect(x, y, fontPointW * fontScale, fontPointH * fontScale, itr2->bg);
          lgfx.setTextColor(itr2->fg, itr2->bg);
          lgfx.drawString(utf8, x, y);
          itr2->dirty = false;
        }

        if(isAscii(itr2->w)){ // TODO: move from editor to util
          x += fontPointW * fontScale;
        }else{
          x += fontPointW * 2 * fontScale;
        }
        if(x > screenWidth){ // not support fold
          break;
        }
      }
      colNo ++;
    }
    x = 0;
    y += fontPointH * fontScale;
    lineNo ++;
  }
}
#endif

void ChrScreen::putString(int x, int y, wchar_t* wp, int16_t fg, int16_t bg){
  while(*wp != 0){
    putChar(x, y, *wp, fg, bg);
    wp ++;
    x ++;
  }
}

void ChrScreen::putString(int x, int y, char* cp, int16_t fg, int16_t bg){
  while(*cp != 0){
    putChar(x, y, *cp, fg, bg);
    cp ++;
    x ++;
  }
}

void ChrScreen::putChar(int x, int y, wchar_t w, int16_t fg, int16_t bg){
  Chr c = {w, fg, bg, true};
  if(y >= lines.size() || x >= lines.at(y).size()){
    return;
  }
  lines.at(y).at(x) = c;
}
void ChrScreen::clearLine(int y, int16_t fg, int16_t bg){
  Chr c = {0, fg, bg, true};
  vector<Chr>::iterator itr2;
  for(itr2 = lines.at(y).begin(); itr2 != lines.at(y).end(); itr2++){
    *itr2 = c;
  }
}
wchar_t ChrScreen::getChar(int x, int y){
  return lines.at(y).at(x).w;
}
int ChrScreen::getMaxLine(){
  return lines.size();
}
