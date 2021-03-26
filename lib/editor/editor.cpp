#include <editor.h>

void Editor::initEditor(){
  line = lines.begin(); // lines
  line = lines.insert(line, vector<wchar_t>());
  colItr = line->begin(); // line
}

void Editor::backSpace(){
  if(colItr != line->begin()){
    colItr --;
    colItr = line->erase(colItr);
  }else{
    if(line == lines.begin())return;
    // marge lines
    vector<wchar_t> ::iterator beginItr = colItr;
    vector<wchar_t> ::iterator endItr = line->end();
    vector<vector<wchar_t>> ::iterator prevLine = line;

    line --;
    int pos = line->end() - line->begin();
    copy(beginItr, endItr, back_inserter(*line));
    line = lines.erase(prevLine);
    line --;
    colItr = line->begin() + pos;
    //lines.shrink_to_fit();
  }
}

void Editor::left(){
  if(colItr != line->begin()){
    colItr --;
  }
}

void Editor::right(){
  if(colItr != line->end()){
    colItr ++;
  }
}

void Editor::up(){
  if(line != lines.begin()){
    int pos = colItr - line->begin();

    line --;
    colItr = line->begin();
    for(;pos != 0 && colItr != line->end(); pos--){
      colItr ++;
    }
  }
}

void Editor::down(){
  if(line + 1 != lines.end()){
    int pos = colItr - line->begin();

    line ++;
    colItr = line->begin();
    for(;pos != 0 && colItr != line->end(); pos--){
      colItr ++;
    }
  }
}

void Editor::enter(){
  vector<wchar_t> newLine;

  copy(colItr, line->end(), back_inserter(newLine));
  while(colItr != line->end()){
    colItr = line->erase(colItr);
  }

  line ++;
  line = lines.insert(line, newLine);

  colItr = line->begin();
}

void Editor::onChar(wchar_t c){
  colItr = line->insert(colItr, c);
  colItr ++;
}

wchar_t table[][5] = {
  {L'あ',L'い',L'う',L'え',L'お'},
  {L'か',L'き',L'く',L'け',L'こ'},
  {L'さ',L'し',L'す',L'せ',L'そ'},
  {L'た',L'ち',L'つ',L'て',L'と'},
  {L'な',L'に',L'ぬ',L'ね',L'の'},
  {L'は',L'ひ',L'ふ',L'へ',L'ほ'},
  {L'ま',L'み',L'む',L'め',L'も'},
  {L'や',L'い',L'ゆ',L'え',L'よ'},
  {L'ら',L'り',L'る',L'れ',L'ろ'},
  {L'わ',L'い',L'う',L'え',L'を'},
  {L'が',L'ぎ',L'ぐ',L'げ',L'ご'},
  {L'ざ',L'じ',L'ず',L'ぜ',L'ぞ'},
  {L'だ',L'ぢ',L'づ',L'で',L'ど'},
  {L'ば',L'び',L'ぶ',L'べ',L'ぼ'},
  {L'ぱ',L'ぴ',L'ぷ',L'ぺ',L'ぽ'},
  {L'ゃ',L'ぃ',L'ゅ',L'ぇ',L'ょ'}
};

uint8_t utf16CharToUtf8(wchar_t utf16, char* utf8){
  uint8_t len = 0;
  if (utf16 < 128) {
    utf8[0] = char(utf16);
    utf8[1] = 0;
    utf8[2] = 0;
    len = 1;
  } else if (utf16 < 2048) {
    utf8[0] = 0xC0 | char(utf16 >> 6);
    utf8[1] = 0x80 | (char(utf16) & 0x3F);
    utf8[2] = 0;
    len = 2;
  } else if (utf16 < 65536) {
    utf8[0] = 0xE0 | char(utf16 >> 12);
    utf8[1] = 0x80 | (char(utf16 >> 6) & 0x3F);
    utf8[2] = 0x80 | (char(utf16) & 0x3F);
    utf8[3] = 0;
    len = 3;
  }
  return len;
}

int GetUtf8ByteCount(char c) {
    if (0 <= uint8_t(c) && uint8_t(c) < 0x80) {
        return 1;
    }
    if (0xC2 <= uint8_t(c) && uint8_t(c) < 0xE0) {
        return 2;
    }
    if (0xE0 <= uint8_t(c) && uint8_t(c) < 0xF0) {
        return 3;
    }
    if (0xF0 <= uint8_t(c) && uint8_t(c) < 0xF8) {
        return 4; // not support yet
    }
    return 0;
}

size_t utf8CharToUtf16(char* utf8, wchar_t* utf16){
  int numBytes = GetUtf8ByteCount(utf8[0]);
  if (numBytes == 0) {
    return 0;
  }
  switch (numBytes) {
    case 1:
      *utf16 = wchar_t(uint8_t(utf8[0]));
      break;
    case 2:
      if ((uint8_t(utf8[0]) & 0x1E) == 0) {
        return 0;
      }

      *utf16 = wchar_t(utf8[0] & 0x1F) << 6;
      *utf16 |= wchar_t(utf8[1] & 0x3F);
      break;
    case 3:
      if ((uint8_t(utf8[0]) & 0x0F) == 0 &&
          (uint8_t(utf8[1]) & 0x20) == 0) {
        return 0;
      }

      *utf16 = wchar_t(utf8[0] & 0x0F) << 12;
      *utf16 |= wchar_t(utf8[1] & 0x3F) << 6;
      *utf16 |= wchar_t(utf8[2] & 0x3F);
      break;
    case 4:
      // not implement
      break;
    default:
      return 0;
  }

  return numBytes;
}


void KanjiEditor::initEditor(){
  Editor::initEditor();
  dictPath = "/SKK-JISYO.S.txt";
}
void KanjiEditor::backSpace(){
  if(kanjiMode == KanjiMode::DIRECT){
    Editor::backSpace();
  }else if(kanjiMode == KanjiMode::KANJI){
    if(rawInputsItr != rawInputs.begin()){
      rawInputsItr --;
    }else{
      // cancel KANJI MODE
      kanjiMode = KanjiMode::DIRECT;
    }
  }
}
void KanjiEditor::right(){
  if(kanjiMode == KanjiMode::HENKAN){
    nextKanji();
  }else{
    Editor::right();
  }
}
void KanjiEditor::left(){
  if(kanjiMode == KanjiMode::HENKAN){
    prevKanji();
  }else{
    Editor::left();
  }
}
void KanjiEditor::up(){
  Editor::up();
}
void KanjiEditor::down(){
  Editor::down();
}
void KanjiEditor::enter(){
  if(kanjiMode == KanjiMode::HENKAN){
    kanjiDecide();
  }else{
    Editor::enter();
  }
}
void KanjiEditor::onChar(wchar_t c){
  if(kanjiMode == KanjiMode::DIRECT){
    Editor::onChar(c);
  }else if(kanjiMode == KanjiMode::KANJI){
    rawInputsItr = rawInputs.insert(rawInputsItr, c);
    rawInputsItr ++;
  }
}

void KanjiEditor::onCharRoma(uint8_t c){
  if(kanjiMode == KanjiMode::HENKAN){
    if(c != ' '){
      kanjiDecide();
    }
  }else if(kanjiMode == KanjiMode::KANJI){
    if(c == ' '){
      kanjiHenkan();
      return;
    }
  }

  switch(c){
    case ' ':
      if(kanjiMode == KanjiMode::HENKAN){
        nextKanji();
        return;
      }
      onChar(c);
      break;
    case 'a':
    case 'i':
    case 'u':
    case 'e':
    case 'o':
              onBoin(c);
              break;
    case 'k':
    case 's':
    case 't':
    case 'n':
    case 'h':
    case 'm':
    case 'y':
    case 'r':
    case 'w':
    case 'g':
    case 'z':
    case 'd':
    case 'b':
    case 'p':
              if(shiin1 == c){
                if(shiin1 == 'n'){
                  onChar(L'ん');
                  shiin1 = 0;
                  shiin2 = 0;
                }else{
                  onChar(L'っ');
                }
              }else if(c == 'y' && shiin1 != 0){
                shiin2 = 'y';
              }else{
                if(shiin1 == 'n'){
                  onChar(L'ん');
                }
                shiin1 = c;
              }
              break;
    default: onChar(c);
  }
}

void KanjiEditor::onBoin(uint8_t c){
  uint8_t b = 0;
  uint8_t pb = 0;
  uint8_t s = 0;
  switch(c){
    case 'a': b = 0; break;
    case 'i': b = 1; break;
    case 'u': b = 2; break;
    case 'e': b = 3; break;
    case 'o': b = 4; break;
  }
  switch(shiin1){
    case 0: s = 0; break;
    case 'k': s = 1; break;
    case 's': s = 2; break;
    case 't': s = 3; break;
    case 'n': s = 4; break;
    case 'h': s = 5; break;
    case 'm': s = 6; break;
    case 'y': s = 7; break;
    case 'r': s = 8; break;
    case 'w': s = 9; break;
    case 'g': s = 10; break;
    case 'z': s = 11; break;
    case 'd': s = 12; break;
    case 'b': s = 13; break;
    case 'p': s = 14; break;

  }
  shiin1 = 0;
  switch(shiin2){
    case 'y':
      pb = b;
      b = 1;
      onChar(table[s][b]);
      onChar(table[15][pb]);
      break;
    case 0:
      onChar(table[s][b]);
  }
  shiin2 = 0;
}

void KanjiEditor::setStartKanjiMode(){
  kanjiMode = KanjiMode::KANJI;
  rawInputs.clear();
  rawInputsItr = rawInputs.begin();
}

void KanjiEditor::kanjiHenkan(){
  kanjiMode = KanjiMode::HENKAN;
  char target[256]; // TODO: check length
  uint16_t pos = 0;
  for(vector<wchar_t>::iterator itr = rawInputs.begin(); itr != rawInputs.end(); itr ++){
    uint8_t len = utf16CharToUtf8(*itr, &target[pos]);
    pos += len;
  }
  target[pos] = 0; // null terminate

  search(target, &kanjiList, (char*)dictPath.c_str());
  kanjiListItr = kanjiList.begin();
  if(kanjiList.empty()){
    kanjiDecide();
  }
}
void KanjiEditor::kanjiDecide(){
  kanjiMode = KanjiMode::DIRECT;
  if(!kanjiList.empty()){
    const char* p = kanjiListItr->c_str();
    while(*p != 0){
      wchar_t utf16;
      size_t n = utf8CharToUtf16((char*)p, &utf16);
      onChar(utf16);
      p += n;
    }
    kanjiList.clear();
  }else{
    // copy rawInputs to line
    vector<wchar_t>::iterator itr;
    for(itr = rawInputs.begin(); itr != rawInputs.end(); itr ++){
      onChar(*itr);
    }
  }
  rawInputs.clear();
  rawInputsItr = rawInputs.begin();
}

void KanjiEditor::nextKanji(){
  kanjiListItr ++;
  if(kanjiListItr == kanjiList.end()){
    kanjiListItr = kanjiList.begin();
  }
}

void KanjiEditor::prevKanji(){
  if(kanjiListItr != kanjiList.begin()){
    kanjiListItr --;
  }else{
    kanjiListItr = kanjiList.end() - 1;
  }
}
