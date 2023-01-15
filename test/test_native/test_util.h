#include <unity.h>
#include <util.h>
#include <string>
#include <vector>

using namespace std;

void util_function_true(void){
  TEST_ASSERT_TRUE(true);
}

void util_simple(void){
  vector<vector<wchar_t>> result;
  vector<wchar_t> target;
  target.push_back(L't');
  target.push_back(L'e');
  target.push_back(L's');
  target.push_back(L't');
  target.push_back(L' ');
  target.push_back(L'f');
  target.push_back(L'o');
  target.push_back(L'o');
  result = split(target);
  TEST_ASSERT_EQUAL_INT32_MESSAGE(2, result.size(), "split size");
  vector<wchar_t> arg1 = result.at(0);
  vector<wchar_t> arg2 = result.at(1);
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L't', arg1.at(0), "split 0,0");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'e', arg1.at(1), "split 0,1");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L's', arg1.at(2), "split 0,2");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L't', arg1.at(3), "split 0,3");

  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'f', arg2.at(0), "split 1,0");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'o', arg2.at(1), "split 1,1");
  TEST_ASSERT_EQUAL_INT16_MESSAGE(L'o', arg2.at(2), "split 1,2");

}

void util_test(){
  RUN_TEST(util_function_true);
  RUN_TEST(util_simple);
}
