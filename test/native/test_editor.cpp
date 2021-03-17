#include <unity.h>
#include <editor.h>
#include <string>

using namespace std;

void test_function_true(void){
  TEST_ASSERT_TRUE(true);
}

int getMaxLines(){
  return lines.size();
}
int getColumnNo(){
  return colItr - line->begin();
}
int getLineNo(){
  return line - lines.begin();
}

void test_editor_init(void){
  initEditor();
  // init lines is 1
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getMaxLines(), "max_lines is 1 in initial");
  // int col is 0
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getColumnNo(), "column 0 in initial");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getLineNo(), "line 0i n initial");
}
void test_editor_move_cursor(void){
  onChar('a');
  onChar('b');
  onChar('c');
  // cursor pos
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "type 3 keys");
  enter();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "enter");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getColumnNo(), "enter");
return;
  onChar('1');
  onChar('2');
  onChar('3');
  onChar('4');
  up();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "type 4 keys in next line and up");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getLineNo(), "check line after up");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check column after up");
  down();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after down");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check column after down");

  right();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after right");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(4, getColumnNo(), "check column after right");

  right();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after over right");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(4, getColumnNo(), "check column after over right");

  backSpace();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after back-space");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check cloumn after back-space");

  right();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after back-space and right");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check line after back-space and right");

  left();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after left");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(2,getColumnNo(), "check line after left");

  left();
  left();
  left();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after over left");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getColumnNo(), "check line after over left");

  onChar('0');
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1, getLineNo(), "check line after insert char");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(1,getColumnNo(), "check line after insert char");

  left();
  backSpace();
  TEST_ASSERT_EQUAL_INT32_MESSAGE(0, getLineNo(), "check line after merge line");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(3, getColumnNo(), "check line after merge line");

  string stdString(lines.at(0).begin(),lines.at(0).end());
  const char *cstr = stdString.c_str();
  TEST_ASSERT_EQUAL_STRING_MESSAGE("abc0123", cstr, "check line0");
}

int main(int argc, char **argv){
  RUN_TEST(test_function_true);
  RUN_TEST(test_editor_init);
  RUN_TEST(test_editor_move_cursor);
}
