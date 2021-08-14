#include <unity.h>
#include <editor.h>
#include <dicttool.h>
#include <string>

using namespace std;
KanjiEditor editor;

void test_function_true(void){
  TEST_ASSERT_TRUE(true);
}

int getMaxLines(){
  return editor.lines.size();
}
int getColumnNo(){
  return editor.colItr - editor.line->begin();
}
int getLineNo(){
  return editor.line - editor.lines.begin();
}

void test_editor_init(void){
  editor.initEditor();
  // init lines is 1
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getMaxLines(), "max_lines is 1 in initial");
  // int col is 0
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getColumnNo(), "column 0 in initial");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getLineNo(), "line 0i n initial");
}
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

void test_editor_rome_conversion(void){
  editor.lines.clear();
  editor.initEditor();

  editor.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'あ', editor.lines.at(0).at(0), "rome a");
  editor.backSpace();

  editor.onCharRoma('k');
  editor.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'か', editor.lines.at(0).at(0), "rome ka");

  editor.backSpace();
  editor.onCharRoma('s');
  editor.onCharRoma('s');
  editor.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'っ', editor.lines.at(0).at(0), "rome ssa");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'さ', editor.lines.at(0).at(1), "rome ssa");

  editor.backSpace();
  editor.backSpace();

  editor.onCharRoma('n');
  editor.onCharRoma('n');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'ん', editor.lines.at(0).at(0), "rome nn");

  editor.backSpace();

  editor.onCharRoma('y');
  editor.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'や', editor.lines.at(0).at(0), "rome ya");

  editor.backSpace();


  editor.onCharRoma('n');
  editor.onCharRoma('y');
  editor.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'に', editor.lines.at(0).at(0), "rome nya1");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'ゃ', editor.lines.at(0).at(1), "rome nya2");
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
  editor.lines.clear();
  editor.initEditor();
  editor.dictPath = "data/SKK-JISYO.S.txt";

  editor.kanjiMode = KanjiMode::KANJI;
  editor.onCharRoma('w');
  editor.onCharRoma('a');
  editor.onCharRoma('n');
  editor.onCharRoma('n');
  editor.onCharRoma(' ');

  TEST_ASSERT_EQUAL_INT32_MESSAGE(4, editor.kanjiList.size(), "check kanjiList size");

  TEST_ASSERT_EQUAL_STRING_MESSAGE("腕", editor.kanjiList.at(0).c_str(), "check result");

  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, editor.kanjiListItr - editor.kanjiList.begin(), "check kanjiList position");
  editor.nextKanji();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, editor.kanjiListItr - editor.kanjiList.begin(), "check kanjiList position after next");
  editor.prevKanji();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, editor.kanjiListItr - editor.kanjiList.begin(), "check kanjiList position after prev");
  editor.prevKanji();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, editor.kanjiListItr - editor.kanjiList.begin(), "check kanjiList position after first prev");
  editor.nextKanji();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, editor.kanjiListItr - editor.kanjiList.begin(), "check kanjiList position last next");
  editor.enter();
}

void test_henkan2(){
  editor.lines.clear();
  editor.initEditor();
  editor.dictPath = "data/SKK-JISYO.S.txt";

  editor.onCharRoma('K');
  editor.onCharRoma('a');
  editor.onCharRoma('n');
  editor.onCharRoma('n');
  editor.onCharRoma('g');
  editor.onCharRoma('a');
  editor.onCharRoma('E');

  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, editor.kanjiList.size(), "check kanjiList size");

  TEST_ASSERT_EQUAL_STRING_MESSAGE("考", editor.kanjiList.at(0).c_str(), "check result");

  editor.enter();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(2, editor.lines.at(0).size(), "check line[0]");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'考', editor.lines.at(0).at(0), "rome kanga");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'え', editor.lines.at(0).at(1), "rome e");

}
void test_henkan_bs(){
  editor.lines.clear();
  editor.initEditor();
  editor.kanjiList.clear();
  editor.kanjiListItr = editor.kanjiList.begin();

  editor.rawInputs.clear();
  editor.rawInputsItr = editor.rawInputs.begin();

  editor.dictPath = "data/SKK-JISYO.S.txt";


  editor.kanjiMode = KanjiMode::KANJI;
  editor.onCharRoma('w');
  editor.onCharRoma('a');
  editor.onCharRoma('n');
  editor.onCharRoma('n');

  TEST_ASSERT_EQUAL_INT32_MESSAGE(2, editor.rawInputs.size(), "check rawInputs");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'わ', editor.rawInputs.at(0), "check rawInputs");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'ん', editor.rawInputs.at(1), "check rawInputs");
  editor.backSpace();
  editor.backSpace();

  TEST_ASSERT_EQUAL_INT32_MESSAGE(KanjiMode::ROME, editor.kanjiMode, "back to rome mode");
}

void test_henkan_ng(){
  editor.lines.clear();
  editor.initEditor();
  editor.kanjiList.clear();
  editor.kanjiListItr = editor.kanjiList.begin();

  editor.rawInputs.clear();
  editor.rawInputsItr = editor.rawInputs.begin();

  editor.dictPath = "data/SKK-JISYO.S.txt";


  editor.kanjiMode = KanjiMode::ROME;
  editor.onCharRoma('K');
  TEST_ASSERT_EQUAL_INT32_MESSAGE(KanjiMode::KANJI, editor.kanjiMode, "go to kanji mode");
  editor.onCharRoma('a');
  editor.onCharRoma('k');
  editor.onCharRoma('u');
  editor.onCharRoma('K');

  TEST_ASSERT_EQUAL_INT32_MESSAGE('k', editor.shiin1, "check shiin1");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, editor.rawInputs.size(), "check rawInputs");

  TEST_ASSERT_EQUAL_INT32_MESSAGE(2, editor.lines.at(0).size(), "check line[0]");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'か', editor.lines.at(0).at(0), "rome ka");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'く', editor.lines.at(0).at(1), "rome ku");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(KanjiMode::ROME, editor.kanjiMode, "back to rome mode");
}


void test_kata(){
  editor.lines.clear();
  editor.initEditor();

  editor.onCharRoma('q');
  editor.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'ア', editor.lines.at(0).at(0), "rome a");
  editor.backSpace();

  editor.onCharRoma('k');
  editor.onCharRoma('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'カ', editor.lines.at(0).at(0), "rome ka");
}
void test_direct(){
  editor.lines.clear();
  editor.initEditor();
  editor.kanjiMode = KanjiMode::ROME;

  editor.onCharRoma('l');
  editor.onChar('a');
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'a', editor.lines.at(0).at(0), "direct a");
}


int main(int argc, char **argv){
  RUN_TEST(test_function_true);
  RUN_TEST(test_editor_init);
  RUN_TEST(test_editor_move_cursor);
  RUN_TEST(test_editor_rome_conversion);
  RUN_TEST(test_kata);
  RUN_TEST(test_direct);

  RUN_TEST(test_dicttool_search);
  RUN_TEST(test_henkan);
  RUN_TEST(test_henkan2);
  RUN_TEST(test_henkan_ng);
  RUN_TEST(test_henkan_bs);
}
