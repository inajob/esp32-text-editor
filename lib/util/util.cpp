#include <util.h>

void to_wchar(vector<wchar_t> line, wchar_t* w, int n){
  for(vector<wchar_t>::iterator itr = line.begin(); itr != line.end(); itr ++){
    *w = *itr;
    w ++;
    n --;
    if(n == 1)break;
  }
  w = 0;
}
vector<vector<wchar_t>> split(vector<wchar_t> line){
  vector<vector<wchar_t>> ret;
  vector<wchar_t> now;

  bool inSpace = false;
  for(vector<wchar_t>::iterator itr = line.begin(); itr != line.end(); itr ++){
    if(*itr == L' '){
      if(inSpace){ // not first space
        // skip
      }else{ // first space
        if(now.size() > 0){
          ret.push_back(now);
        }
        now.clear();
      }
      inSpace = true;
    }else{
      now.push_back(*itr);
    }
  }
  if(now.size() > 0){
    ret.push_back(now);
  }
  return ret;
}

