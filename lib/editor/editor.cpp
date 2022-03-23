#include <editor.h>

void Editor::init(){
  lines.clear();
  line = lines.begin(); // lines
  line = lines.insert(line, vector<wchar_t>());
  colItr = line->begin(); // line
}

void Editor::onkeydown(char key, char c, bool ctrl){
  switch(key){
    case 0x2A: // BS
      backSpace();
    break;
    case 0x50: // <-
      left();
    break;
    case 0x4F: // ->
      right();
    break;
    case 0x52: // ^
      up();
    break;
    case 0x51: // v
      down();
    break;
    default:
      if(c == '\r'){
        enter();
      }else{
        onChar(c);
      }
  }
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

void Editor::load(){
#ifdef ESP32
  init();
  File fp;
  fp = SPIFFS.open(filename , "r");
  while(fp.available()){
    char c1 = fp.read();
    char c2 = fp.read();
    printf("load %x,%x\n", c1, c2);
    wchar_t w = (c1 << 8) | (c2);
    if(c1 == 0 && c2 == '\n'){
      enter();
    }else{
      onChar(w);
    }
  }
  fp.close();
#endif
}
void Editor::save(){
#ifdef ESP32
  vector<vector<wchar_t>>::iterator itr;
  vector<wchar_t>::iterator itr2;

  File fp;
  fp = SPIFFS.open(filename , "w");
  for(itr = lines.begin(); itr != lines.end(); itr ++){
    for(itr2 = itr->begin(); itr2 != itr->end(); itr2 ++){
      char c1 = (*itr2 >> 8);
      char c2 = *itr2 & 0xff;
      printf("save %x,%x\n", c1, c2);
      fp.write(0xff & c1); // save as UTF16
      fp.write(0xff & c2); // save as UTF16
    }
    fp.write(0x00);
    fp.write('\n');
  }
  fp.close();
#endif
}

wchar_t KanjiEditor::table[][5] = {
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
  {L'ゃ',L'ぃ',L'ゅ',L'ぇ',L'ょ'},
  {L'っ',L'ん',L'-',L'-',L'-'}, // 16
  {L'ぁ',L'ぃ',L'ぅ',L'ぇ',L'ぉ'}
};
wchar_t KanjiEditor::kata_table[][5] = {
  {L'ア',L'イ',L'ウ',L'エ',L'オ'},
  {L'カ',L'キ',L'ク',L'ケ',L'コ'},
  {L'サ',L'シ',L'ス',L'セ',L'ソ'},
  {L'タ',L'チ',L'ツ',L'テ',L'ト'},
  {L'ナ',L'ニ',L'ヌ',L'ネ',L'ノ'},
  {L'ハ',L'ヒ',L'フ',L'ヘ',L'ホ'},
  {L'マ',L'ミ',L'ム',L'メ',L'モ'},
  {L'ヤ',L'イ',L'ユ',L'エ',L'ヨ'},
  {L'ラ',L'リ',L'ル',L'レ',L'ロ'},
  {L'ワ',L'イ',L'ウ',L'エ',L'ヲ'},
  {L'ガ',L'ギ',L'グ',L'ゲ',L'ゴ'},
  {L'ザ',L'ジ',L'ズ',L'ゼ',L'ゾ'},
  {L'ダ',L'ヂ',L'ヅ',L'デ',L'ド'},
  {L'バ',L'ビ',L'ブ',L'ベ',L'ボ'},
  {L'パ',L'ピ',L'プ',L'ペ',L'ポ'},
  {L'ャ',L'ィ',L'ュ',L'ェ',L'ョ'},
  {L'ッ',L'ン',L'-',L'-',L'-'},
  {L'ァ',L'ィ',L'ゥ',L'ェ',L'ォ'}
};

bool isAscii(wchar_t utf16){
  return utf16 >= 0 && utf16 <= 0x7e;
}

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


void KanjiEditor::init(){
  Editor::init();
  dictPath = "/SKK-JISYO.S.txt";
}
void KanjiEditor::onkeydown(char key, char c, bool ctrl){
  switch(key){
    case 0x2A: // BS
      backSpace();
    break;
    case 0x50: // <-
      left();
    break;
    case 0x4F: // ->
      right();
    break;
    case 0x52: // ^
      up();
    break;
    case 0x51: // v
      down();
    break;
    default:
      if(c == '\r'){
        enter();
      }else if(c == 'x' && ctrl){
        terminate();
        filename[0] = 0; // todo: implement clearFileName
      }else{
        onCharRoma(c,ctrl);
      }
  }
}

void KanjiEditor::backSpace(){
  if(kanjiMode == KanjiMode::ROME || kanjiMode == KanjiMode::KATA || kanjiMode == KanjiMode::DIRECT){
    if(shiin2 != 0){
      shiin2 = 0;
      return;
    }
    if(shiin1 != 0){
      shiin1 = 0;
      return;
    }
    Editor::backSpace();
  }else if(kanjiMode == KanjiMode::KANJI){
    if(rawInputsItr != rawInputs.begin()){
      rawInputsItr --;
      rawInputsItr = rawInputs.erase(rawInputsItr);
      if(rawInputsItr == rawInputs.begin()){
        kanjiMode = KanjiMode::ROME;
      }
    }else{
      // cancel KANJI MODE
      kanjiMode = KanjiMode::ROME;
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
  if(kanjiMode == KanjiMode::ROME || kanjiMode == KanjiMode::KATA || kanjiMode == KanjiMode::DIRECT){
    Editor::onChar(c);
  }else if(kanjiMode == KanjiMode::KANJI){
    rawInputsItr = rawInputs.insert(rawInputsItr, c);
    rawInputsItr ++;
  }
}

wchar_t KanjiEditor::getKana(int r, int c){
  if(kanjiMode == KanjiMode::KATA){
    return kata_table[r][c];
  }
  return table[r][c];
}
void KanjiEditor::onCharRoma(uint8_t c, bool ctrl){
  if(ctrl){
    if(c == 'j'){
      kanjiMode = KanjiMode::ROME;
    }
    if(c == 's'){
      save();
    }
    if(c == 'l'){
      load();
    }
    return;
  }
  if(kanjiMode == KanjiMode::DIRECT){
    onChar(c);
    return;
  }
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

  if(isalpha(c) && c == toupper(c)){ // uppar case
    if(kanjiMode == KanjiMode::ROME || kanjiMode == KanjiMode::KATA){ // start kanji mode
      setStartKanjiMode();
      c = tolower(c);
    }else if(kanjiMode == KanjiMode::KANJI){ // last 1 char and henkan
      c = tolower(c);
      onChar(c);
      kanjiHenkan();
      return;
    }else if(kanjiMode == KanjiMode::HENKAN){ // decide and next kanji
      kanjiDecide();
      setStartKanjiMode();
      c = tolower(c);
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
    case 'x':
              if(shiin1 == c){
                if(shiin1 == 'n'){
                  onChar(getKana(16, 1)); // ん
                  shiin1 = 0;
                  shiin2 = 0;
                }else{
                  onChar(getKana(16, 0)); // っ
                }
              }else if(c == 'y' && shiin1 != 0){
                shiin2 = 'y';
              }else{
                if(shiin1 == 'n'){
                  onChar(getKana(16, 1)); // ん
                }
                shiin1 = c;
              }
              break;
    case 'l':
      kanjiMode = KanjiMode::DIRECT;
      break;
    case 'q':
      kanjiMode = KanjiMode::KATA;
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
    case 'x': s = 17; break;

  }
  shiin1 = 0;
  switch(shiin2){
    case 'y':
      pb = b;
      b = 1;
      onChar(getKana(s, b));
      onChar(getKana(15, pb));
      break;
    case 0:
      onChar(getKana(s, b));
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
  kanjiMode = KanjiMode::ROME;
  if(!kanjiList.empty()){
    const char* p = kanjiListItr->c_str();
    while(*p != 0){
      wchar_t utf16;
      size_t n = utf8CharToUtf16((char*)p, &utf16);
      onChar(utf16);
      p += n;
    }
    kanjiList.clear();
    // copy tail alpha at rawInputs to line
    vector<wchar_t>::iterator itr;
    for(itr = rawInputs.begin(); itr != rawInputs.end(); itr ++){
      if(*itr < 128 && isalpha(char(*itr))){
        printf("add char %c\n", char(*itr));
        onCharRoma(char(*itr), false);
      }
    }
  }else{
    // copy rawInputs to line
    vector<wchar_t>::iterator itr;
    for(itr = rawInputs.begin(); itr != rawInputs.end(); itr ++){
      if(*itr < 128 && isalpha(char(*itr))){
        onCharRoma(char(*itr), false);
      }else{
        onChar(*itr);
      }
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

void KanjiEditor::draw(){
  // draw decided characters
  vector<vector<wchar_t>>::iterator itr;
  vector<wchar_t>::iterator itr2;
  int x = 0, y = 0;
  int cursorX = 0, cursorY = 0;
  for(itr = lines.begin(); itr != lines.end(); itr ++){
    chrScreen->clearLine(y, TFT_WHITE, TFT_BLACK);
    chrScreen->putChar(0, y, (wchar_t)('0' + y/10), TFT_WHITE, TFT_BLACK);
    chrScreen->putChar(1, y, (wchar_t)('0' + y%10), TFT_WHITE, TFT_BLACK);
    x += 3;
    for(itr2 = itr->begin(); itr2 != itr->end(); itr2 ++){
      if(itr2 == colItr){ // cursor
        chrScreen->putChar(x, y, *itr2, TFT_BLACK, TFT_WHITE);
        cursorX = x;
        cursorY = y;
      }else{
        chrScreen->putChar(x, y, *itr2, TFT_WHITE, TFT_BLACK);
      }
      x ++;
    }
    if(line == itr && itr->end() == colItr){ // cursor
      chrScreen->putChar(x, y, 0, TFT_BLACK, TFT_WHITE);
      cursorX = x;
      cursorY = y;
    }
    x = 0;
    y ++;
    if(y == chrScreen->getMaxLine() - 2){
      break;
    }
  }
  chrScreen->clearLine(y, TFT_WHITE, TFT_BLACK);

  // draw un-decided characters
  x = cursorX;
  y = cursorY;
  bool hasRawInputs = false;
  for(itr2 = rawInputs.begin(); itr2 != rawInputs.end(); itr2 ++){
    char utf8[4];
    chrScreen->putChar(x, y, *itr2, TFT_BLACK, TFT_WHITE);
    x ++;
    hasRawInputs = true;
  }
  if(!hasRawInputs){
    if(shiin1 != 0){
      chrScreen->putChar(x, y,(wchar_t)shiin1, TFT_BLACK, TFT_WHITE);
      x ++;
    }
    if(shiin2 != 0){
      chrScreen->putChar(x, y, (wchar_t)shiin2, TFT_BLACK, TFT_WHITE);
      x ++;
    }
  }

  // mode line
  switch(kanjiMode){
    case KanjiMode::DIRECT: chrScreen->putString(0, chrScreen->getMaxLine() - 1, L"[A]", TFT_BLACK, TFT_WHITE); break;
    case KanjiMode::KATA:   chrScreen->putString(0, chrScreen->getMaxLine() - 1, L"[ア]", TFT_BLACK, TFT_WHITE); break;
    case KanjiMode::ROME:   chrScreen->putString(0, chrScreen->getMaxLine() - 1, L"[あ]", TFT_BLACK, TFT_WHITE); break;
    case KanjiMode::KANJI:  chrScreen->putString(0, chrScreen->getMaxLine() - 1, L"[漢]", TFT_BLACK, TFT_WHITE); break;
    case KanjiMode::HENKAN: break;
  }

  chrScreen->putString(5, chrScreen->getMaxLine() - 1, filename, TFT_BLACK, TFT_WHITE);

  x = 0;
  y = chrScreen->getMaxLine() - 2;
  chrScreen->clearLine(y, TFT_WHITE, TFT_BLACK);
  if(kanjiMode == KanjiMode::HENKAN){
    for(vector<string>:: iterator kanji = kanjiList.begin(); kanji != kanjiList.end(); kanji ++){
      int16_t fg = TFT_WHITE;
      int16_t bg = TFT_BLACK;
      if(kanji == kanjiListItr){
        fg = TFT_BLACK;
        bg = TFT_WHITE;
      }
      // kanji is utf8
      const char* k = kanji->c_str();
      wchar_t w;
      while(*k != 0){
        size_t n = utf8CharToUtf16((char*)k, &w);
        chrScreen->putChar(x, y, w, fg, bg);
        x ++;
        k += n;
      }
      chrScreen->putChar(x, y, L' ', fg, bg);
      x ++;
      // TODO: overflow x
    }
  }

}
