#include <unity.h>
//#include <editor.h>
#include <chrscreen.h>
#include <shell.h>
#include <fep.h>
#include <dicttool.h>
#include <string>
#include "test_chrscreen.h"
#include "test_util.h"

using namespace std;
//KanjiEditor editor;
KanjiFep fep;
Shell shell;

void test_function_true(void){
  TEST_ASSERT_TRUE(true);
}
/*
int getMaxLines(){
  return editor.lines.size();
}
*/
int getColumnNo(){
  return fep.colItr - fep.line.begin();
}
/*
int getLineNo(){
  return editor.line - editor.lines.begin();
}
*/

void test_fep_init(void){
  chrScreen.init(320, 240);
  shell.setChrScreen(&chrScreen);
  fep.setChrScreen(&chrScreen);
  fep.setParentTask(&shell);
  shell.init();
  fep.init();
  //fep.setParentTask(&shell);
  // int col is 0
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getColumnNo(), "column 0 in initial");
}

/*
void test_editor_move_cursor(void){
  editor.backSpace();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getLineNo(), "check line after bs");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getColumnNo(), "check column after bs");

  editor.onChar('a');
  editor.onChar('b');
  editor.onChar('c');
  // cursor pos
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "type 3 keys");
  editor.enter();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "enter");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getColumnNo(), "enter");

  editor.onChar('1');
  editor.onChar('2');
  editor.onChar('3');
  editor.onChar('4');
  editor.up();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "type 4 keys in next line and up");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getLineNo(), "check line after up");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check column after up");
  editor.down();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after down");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check column after down");

  editor.right();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after right");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(4, getColumnNo(), "check column after right");

  editor.right();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after over right");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(4, getColumnNo(), "check column after over right");

  editor.backSpace();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after back-space");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check cloumn after back-space");

  editor.right();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after back-space and right");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check line after back-space and right");

  editor.left();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after left");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(2,getColumnNo(), "check line after left");

  editor.left();
  editor.left();
  editor.left();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after over left");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getColumnNo(), "check line after over left");

  editor.onChar('0');
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after insert char");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1,getColumnNo(), "check line after insert char");

  editor.left();
  editor.backSpace();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getLineNo(), "check line after merge line");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check column after merge line");

  string stdString(editor.lines.at(0).begin(),editor.lines.at(0).end());
  const char *cstr = stdString.c_str();
  TEST_ASSERT_EQUAL_STRING_MESSAGE("abc0123", cstr, "check line0");
}
*/

void test_fep_rome_conversion(void){
  fep.init();

  fep.onCharRoma('a', false);
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'あ', shell.rawInputs.at(0), "rome a");
  fep.backSpace();

  fep.onCharRoma('k');
  fep.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'か', shell.rawInputs.at(0), "rome ka");

  fep.backSpace();
  fep.onCharRoma('s');
  fep.onCharRoma('s');
  fep.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'っ', shell.rawInputs.at(0), "rome ssa");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'さ', shell.rawInputs.at(1), "rome ssa");

  fep.backSpace();
  fep.backSpace();

  fep.onCharRoma('n');
  fep.onCharRoma('n');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'ん', shell.rawInputs.at(0), "rome nn");

  fep.backSpace();

  fep.onCharRoma('y');
  fep.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'や', shell.rawInputs.at(0), "rome ya");

  fep.backSpace();


  fep.onCharRoma('n');
  fep.onCharRoma('y');
  fep.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'に', shell.rawInputs.at(0), "rome nya1");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'ゃ', shell.rawInputs.at(1), "rome nya2");
}

void test_dicttool_search(){
  vector<string> results;
  search("わるs", &results,"data/SKK-JISYO.S.txt");
  //printf("result: %s", results);
  TEST_ASSERT_EQUAL_STRING_MESSAGE("悪", results.at(0).c_str(), "check result");

  results.clear();
  search("れき", &results,"data/SKK-JISYO.S.txt");
  //printf("result: %x\n", results.at(0).c_str()[0]);
  TEST_ASSERT_EQUAL_STRING_MESSAGE("歴", results.at(0).c_str(), "check result2");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("暦", results.at(1).c_str(), "check result2-2");

  results.clear();
  search("わん", &results,"data/SKK-JISYO.S.txt");
  //printf("result: %s", results);
  TEST_ASSERT_EQUAL_STRING_MESSAGE("腕", results.at(0).c_str(), "check result3");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("碗", results.at(1).c_str(), "check result3-2");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("湾", results.at(2).c_str(), "check result3-3");
  TEST_ASSERT_EQUAL_STRING_MESSAGE("椀", results.at(3).c_str(), "check result3-4");

}

void test_henkan(){
  fep.init();
  fep.dictPath = "data/SKK-JISYO.S.txt";

  fep.kanjiMode = KanjiMode::KANJI;
  fep.onCharRoma('w');
  fep.onCharRoma('a');
  fep.onCharRoma('n');
  fep.onCharRoma('n');
  fep.onCharRoma(' ');

  TEST_ASSERT_EQUAL_INT32_MESSAGE(4, fep.kanjiList.size(), "check kanjiList size");

  TEST_ASSERT_EQUAL_STRING_MESSAGE("腕",fep.kanjiList.at(0).c_str(), "check result");

  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, fep.kanjiListItr - fep.kanjiList.begin(), "check kanjiList position");
  fep.nextKanji();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, fep.kanjiListItr - fep.kanjiList.begin(), "check kanjiList position after next");
  fep.prevKanji();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, fep.kanjiListItr - fep.kanjiList.begin(), "check kanjiList position after prev");
  fep.prevKanji();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, fep.kanjiListItr - fep.kanjiList.begin(), "check kanjiList position after first prev");
  fep.nextKanji();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, fep.kanjiListItr - fep.kanjiList.begin(), "check kanjiList position last next");
  fep.enter();
}

void test_henkan2(){
  fep.init();
  fep.dictPath = "data/SKK-JISYO.S.txt";

  fep.onCharRoma('K');
  fep.onCharRoma('a');
  fep.onCharRoma('n');
  fep.onCharRoma('n');
  fep.onCharRoma('g');
  fep.onCharRoma('a');
  fep.onCharRoma('E');

  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, fep.kanjiList.size(), "check kanjiList size");

  TEST_ASSERT_EQUAL_STRING_MESSAGE("考", fep.kanjiList.at(0).c_str(), "check result");

  fep.enter();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(2, shell.rawInputs.size(), "check line[0]");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'考', shell.rawInputs.at(0), "rome kanga");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'え', shell.rawInputs.at(1), "rome e");
}

void test_henkan_bs(){
  fep.init();
  fep.kanjiList.clear();
  fep.kanjiListItr = fep.kanjiList.begin();

  fep.rawInputs.clear();
  fep.rawInputsItr = fep.rawInputs.begin();

  fep.dictPath = "data/SKK-JISYO.S.txt";


  fep.kanjiMode = KanjiMode::KANJI;
  fep.onCharRoma('w');
  fep.onCharRoma('a');
  fep.onCharRoma('n');
  fep.onCharRoma('n');

  TEST_ASSERT_EQUAL_INT32_MESSAGE(2, fep.rawInputs.size(), "check rawInputs");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'わ', fep.rawInputs.at(0), "check rawInputs");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'ん', fep.rawInputs.at(1), "check rawInputs");
  fep.backSpace();
  fep.backSpace();

  TEST_ASSERT_EQUAL_INT32_MESSAGE(KanjiMode::ROME, fep.kanjiMode, "back to rome mode");
}

void test_henkan_ng(){
  fep.init();
  fep.kanjiList.clear();
  fep.kanjiListItr = fep.kanjiList.begin();

  fep.rawInputs.clear();
  fep.rawInputsItr = fep.rawInputs.begin();

  fep.dictPath = "data/SKK-JISYO.S.txt";


  fep.kanjiMode = KanjiMode::ROME;
  fep.onCharRoma('K');
  TEST_ASSERT_EQUAL_INT32_MESSAGE(KanjiMode::KANJI, fep.kanjiMode, "go to kanji mode");
  fep.onCharRoma('a');
  fep.onCharRoma('k');
  fep.onCharRoma('u');
  fep.onCharRoma('K');

  TEST_ASSERT_EQUAL_INT32_MESSAGE('k', fep.shiin1, "check shiin1");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, fep.rawInputs.size(), "check rawInputs");

  TEST_ASSERT_EQUAL_INT32_MESSAGE(2, shell.rawInputs.size(), "check line[0]");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'か', shell.rawInputs.at(0), "rome ka");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'く', shell.rawInputs.at(1), "rome ku");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(KanjiMode::ROME, fep.kanjiMode, "back to rome mode");
}

void test_kata(){
  fep.init();

  fep.onCharRoma('q');
  fep.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'ア', shell.rawInputs.at(0), "rome a");
  fep.backSpace();

  fep.onCharRoma('k');
  fep.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'カ', shell.rawInputs.at(0), "rome ka");
}
void test_direct(){
  fep.init();
  fep.kanjiMode = KanjiMode::ROME;

  fep.onCharRoma('l');
  fep.onChar('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'a', shell.rawInputs.at(0), "direct a");
}

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

int main(int argc, char **argv){
  RUN_TEST(test_function_true);
  RUN_TEST(test_fep_init);
  //RUN_TEST(test_editor_move_cursor);
  RUN_TEST(test_fep_rome_conversion);
  RUN_TEST(test_kata);
  RUN_TEST(test_direct);

  RUN_TEST(test_dicttool_search);
  RUN_TEST(test_henkan);
  RUN_TEST(test_henkan2);
  RUN_TEST(test_henkan_ng);
  RUN_TEST(test_henkan_bs);
  chrscreen_test();
  util_test();
}
