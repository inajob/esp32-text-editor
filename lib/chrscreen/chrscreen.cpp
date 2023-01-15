#include <chrscreen.h>

// to use utf16CharToUtf8
#include <fep.h>

// w means the number of half characters
void ChrScreen::init(int w, int h){
  vector<vector<Chr>> ::iterator line;
  screenWidth = w;
  cursorCol = 0;
  cursorColPos = 0;
  cursorLine = 0;

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
  lgfx.setTextSize(fontScale, fontScale);
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

void ChrScreen::setCursor(int col, int line){
  cursorCol = col;
  cursorLine = line;
  // TODO: cursorColPos
}

void ChrScreen::putString(wchar_t* wp, int16_t fg, int16_t bg){
  while(*wp != 0){
    putChar(*wp, fg, bg);
    wp ++;
  }
}

void ChrScreen::putString(char* cp, int16_t fg, int16_t bg){
  while(*cp != 0){
    putChar(*cp, fg, bg);
    cp ++;
  }
}

void ChrScreen::putChar(wchar_t w, int16_t fg, int16_t bg){
  Chr c = {w, fg, bg, true};
  //if(cursorColPos + getCharSize(w) > screenWidth){ // folding
  //  cursorColPos = 0;
  //  cursorCol = 0;
  //  cursorLine ++;
  //}
  if(cursorLine >= lines.size() || cursorCol >= lines.at(cursorLine).size()){
    return;
  }

  lines.at(cursorLine).at(cursorCol) = c;
  cursorCol ++;
  cursorColPos += getCharSize(w);
}
void ChrScreen::back(){
  if(cursorCol > 0){
    cursorCol --;
  }
}
void ChrScreen::nextLine(){
  cursorCol = 0;
  cursorColPos = 0;
  cursorLine ++;
  if(cursorLine >= lines.size()){
    cursorLine = 0;
  }
}
void ChrScreen::clearLine(int line, int16_t fg, int16_t bg){
  Chr c = {0, fg, bg, true};
  vector<Chr>::iterator itr2;
  for(itr2 = lines.at(line).begin(); itr2 != lines.at(line).end(); itr2++){
    *itr2 = c;
  }
}
wchar_t ChrScreen::getChar(int col, int line){
  return lines.at(line).at(col).w;
}
int ChrScreen::getCharSize(wchar_t w){
  if(isAscii(w)){
    return fontPointW * fontScale;
  }else{
    return fontPointW * 2 * fontScale;
  }
}
int ChrScreen::getMaxLine(){
  return lines.size();
}
int ChrScreen::getMaxColumn(){
  return screenWidth / fontPointW;
}
