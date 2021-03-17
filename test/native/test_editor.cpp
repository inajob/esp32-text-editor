#include <unity.h>

void test_function_true(void){
  TEST_ASSERT_TRUE(true);
}

int main(int argc, char **argv){
  RUN_TEST(test_function_true);
}
