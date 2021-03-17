#include <editor.h>

vector<vector<wchar_t>> lines;

vector<vector<wchar_t>> ::iterator line;
vector<wchar_t> ::iterator colItr;

uint8_t shiin1 = 0;


void initEditor(){
  line = lines.begin(); // lines
  //lines.push_back(vector<wchar_t>());
  line = lines.insert(line, vector<wchar_t>());
  colItr = line->begin(); // line
}

void backSpace(){
  if(colItr != line->begin()){
    colItr --;
    colItr = line->erase(colItr);
  }else{
    // marge lines
    vector<wchar_t> ::iterator beginItr = colItr;
    vector<wchar_t> ::iterator endItr = line->end();
    vector<vector<wchar_t>> ::iterator prevLine = line;

    line --;
    int pos = endItr - beginItr;
    copy(beginItr, endItr, back_inserter(*line));
    line = lines.erase(prevLine);
    line --;
    colItr = line->begin() + pos;
    //lines.shrink_to_fit();
  }
}

void left(){
  if(colItr != line->begin()){
    colItr --;
  }
}

void right(){
  if(colItr != line->end()){
    colItr ++;
  }
}

void up(){
  if(line != lines.begin()){
    int pos = colItr - line->begin();

    line --;
    colItr = line->begin();
    for(;pos != 0 && colItr != line->end(); pos--){
      colItr ++;
    }
  }
}

void down(){
  if(line + 1 != lines.end()){
    int pos = colItr - line->begin();

    line ++;
    colItr = line->begin();
    for(;pos != 0 && colItr != line->end(); pos--){
      colItr ++;
    }
  }
}

void enter(){
  vector<wchar_t> newLine;

  copy(colItr, line->end(), back_inserter(newLine));
  while(colItr != line->end()){
    colItr = line->erase(colItr);
  }

  line ++;
  line = lines.insert(line, newLine);

  colItr = line->begin();
}

void onChar(wchar_t c){
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
  {L'ぱ',L'ぴ',L'ぷ',L'ぺ',L'ぽ'}
};

void onBoin(uint8_t c){
  uint8_t b = 0;
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
  onChar(table[s][b]);
}
void onCharRoma(uint8_t c){
  switch(c){
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
                }else{
                  onChar(L'っ');
                }
              }
              shiin1 = c; break;
    default: onChar(c);
  }
}
