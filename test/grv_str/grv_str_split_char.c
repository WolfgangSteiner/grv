#include "grv/grv_test.h"
#include "grv/grv.h"

GRV_TEST_BEGIN_NEW(grv_str_split_char)
    grv_str_t s = grv_str_ref("/this//is a/test");
    grv_strarr_t split = grv_str_split_char(s, '/');
    GRV_TEST_EQUAL_INT(split.size, 5);
    GRV_TEST_EQUAL_STR(split.arr[0], "");
    GRV_TEST_EQUAL_STR(split.arr[1], "this");
    GRV_TEST_EQUAL_STR(split.arr[2], "");
    GRV_TEST_EQUAL_STR(split.arr[3], "is a");
    GRV_TEST_EQUAL_STR(split.arr[4], "test");
GRV_TEST_END_NEW()
