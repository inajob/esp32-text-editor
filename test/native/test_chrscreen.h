#include <unity.h>
#include <editor.h>
#include <chrscreen.h>
#include <string>

using namespace std;

ChrScreen chrScreen;

void chrscreen_function_true(void){
  TEST_ASSERT_TRUE(true);
}

void chrscreen_simple(void){
  chrScreen.init(10*6, 40*12);
  TEST_ASSERT_EQUAL_INT32_MESSAGE(40, chrScreen.lines.size(), "lines size");
  TEST_ASSERT_EQUAL_INT32_MESSAGE(10, chrScreen.lines.begin()->size(), "line size");

  chrScreen.putChar(0, 0, L'a', 0, 0);
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'a', chrScreen.getChar(0, 0), "put/get char");
  chrScreen.putChar(0, 1, L'あ', 0, 0);
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'あ', chrScreen.getChar(0, 1), "put/get wide char");

  chrScreen.putString(0, 0, L"test", 0, 0);
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L't', chrScreen.getChar(0, 0), "putString");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'e', chrScreen.getChar(1, 0), "putString");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L's', chrScreen.getChar(2, 0), "putString");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L't', chrScreen.getChar(3, 0), "putString");
}

void chrscreen_test(){
  RUN_TEST(chrscreen_function_true);
  RUN_TEST(chrscreen_simple);
}
