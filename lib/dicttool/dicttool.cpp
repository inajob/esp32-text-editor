#include <dicttool.h>

void search(char* target, vector<string>* results, char* path){
#ifdef ESP32
  File fp;
#else
  FILE* fp;
#endif
  const size_t line_size = 1024;
  char line[1024];
  char* linePtr = line; // TODO: more smart way?
  char* targetPtr = target;
#ifdef ESP32
  fp = SPIFFS.open(path, "r");
#else
  fp = fopen(path, "r");
#endif
  int i = 0;
  // TODO: check fp is null;
#ifdef ESP32
  while(fp.available()){
    String lineStr = fp.readStringUntil('\n');
    char* line = (char*)lineStr.c_str();
#else
  while(fgets(line, line_size, fp) != NULL) {
#endif
    i++;
    linePtr = line;
    if(*linePtr == ';'){ // skip comment
      continue;
    }
    targetPtr = target;
    bool hit = false;
    while(*linePtr != 0){
      if(*linePtr == ' '){ // end of okuri-ari
        if(*targetPtr == 0){
          hit = true;
        }
        break;
      }
      if(*linePtr != *targetPtr){
        break;
      }
      linePtr ++;
      targetPtr ++;
    }
    if(hit == false)continue;

    // here is hit
    linePtr ++; // skip ` `
    linePtr ++; // skip first `/`

    while(*linePtr != 0 && *linePtr != '\r' && *linePtr != '\n'){
      string result;

      while(*linePtr != '/'){
        result += *linePtr;
        linePtr ++;
      }
      result += '\0';
      results->push_back(result);
      linePtr ++; // skip `/`
    }
    break;
  }
#ifdef ESP32
  fp.close();
#else
  fclose(fp);
#endif
}
