#include <unity.h>
#include <editor.h>
#include <string>

using namespace std;

void test_function_true(void){
  TEST_ASSERT_TRUE(true);
}

void check_max_lines(int n, const char* m){
  TEST_ASSERT_EQUAL_INT32_MESSAGE(n, lines.size(), m);
}
void check_column(int n, const char* m){
  TEST_ASSERT_EQUAL_INT32_MESSAGE(n, colItr - line->begin(), m);
}
void check_line(int n, const char* m){
  TEST_ASSERT_EQUAL_INT32_MESSAGE(n, line - lines.begin(), m);
}

void test_editor_init(void){
  initEditor();
  // init lines is 1
  check_max_lines(1, "max_lines is 1 in initial");
  // int col is 0
  check_column(0, "column 0 in initial");
  check_line(0, "line 0i n initial");
}
void test_editor_move_cursor(void){
  onChar('a');
  onChar('b');
  onChar('c');
  // cursor pos
  check_column(3, "type 3 keys");
  enter();
  check_line(1, "enter");
  check_column(0, "enter");
return;
  onChar('1');
  onChar('2');
  onChar('3');
  onChar('4');
  up();
  check_column(3, "type 4 keys in next line and up");
  check_line(0, "check line after up");
  check_column(3, "check column after up");
  down();
  check_line(1, "check line after down");
  check_column(3, "check column after down");

  right();
  check_line(1, "check line after right");
  check_column(4, "check column after right");

  right();
  check_line(1, "check line after over right");
  check_column(4, "check column after over right");

  backSpace();
  check_line(1, "check line after back-space");
  check_column(3, "check cloumn after back-space");

  right();
  check_line(1, "check line after back-space and right");
  check_column(3, "check line after back-space and right");

  left();
  check_line(1, "check line after left");
  check_column(2, "check line after left");

  left();
  left();
  left();
  check_line(1, "check line after over left");
  check_column(0, "check line after over left");

  onChar('0');
  check_line(1, "check line after insert char");
  check_column(1, "check line after insert char");

  left();
  backSpace();
  check_line(0, "check line after merge line");
  check_column(3, "check line after merge line");


  string stdString(lines.at(0).begin(),lines.at(0).end());
  const char *cstr = stdString.c_str();
  TEST_ASSERT_EQUAL_STRING_MESSAGE("abc0123", cstr, "check line0");
}

int main(int argc, char **argv){
  RUN_TEST(test_function_true);
  RUN_TEST(test_editor_init);
  RUN_TEST(test_editor_move_cursor);
}
