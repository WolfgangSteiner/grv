#include "grv/grv_test.h"

GRV_TEST_BEGIN(assert_true_false)
    GRV_TEST_TRUE(true);
    GRV_TEST_FALSE(false);
GRV_TEST_END()

GRV_TEST_BEGIN(assert_equal)
    GRV_TEST_EQUAL_INT(1, 1);
    GRV_TEST_NOT_EQUAL_INT(1, 2);
GRV_TEST_END()

GRV_TEST_BEGIN(assert_equal_str)
    GRV_TEST_EQUAL_STR(grv_str_ref("Hello"), "Hello");
    GRV_TEST_NOT_EQUAL_STR(grv_str_ref("Hello"), "World");
GRV_TEST_END()

int main(void) {
    GRV_TEST_INIT(test);
    GRV_TEST_RUN(assert_true_false);
    GRV_TEST_RUN(assert_equal);
    GRV_TEST_RUN(assert_equal_str);
    GRV_TEST_FINISH();
}