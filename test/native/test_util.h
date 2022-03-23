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
}

void util_test(){
  RUN_TEST(util_function_true);
  RUN_TEST(util_simple);
}
