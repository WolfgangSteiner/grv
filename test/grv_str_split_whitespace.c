#include "grv/grv_test.h"
#include "grv/grv_str.h"
#include "grv/grv_strarr.h"

GRV_TEST_BEGIN_NEW(grv_str_split_whitespace)
    grv_str_t s = grv_str_ref(" This is a test.    ");
    grv_strarr_t arr = grv_str_split_whitespace(s);
    GRV_TEST_EQUAL_INT(arr.size, 6);
    GRV_TEST_EQUAL_STR(arr.arr[0], "");
    GRV_TEST_EQUAL_STR(arr.arr[1], "This");
    GRV_TEST_EQUAL_STR(arr.arr[2], "is");
    GRV_TEST_EQUAL_STR(arr.arr[3], "a");
    GRV_TEST_EQUAL_STR(arr.arr[4], "test.");
    GRV_TEST_EQUAL_STR(arr.arr[5], "");

    s = grv_str_ref("wlan0: 0000   55.  -55.  -256        0      0      0      0     63        0");
    arr = grv_str_split_whitespace(s);
    GRV_TEST_EQUAL_INT(arr.size, 11);
    GRV_TEST_EQUAL_STR(arr.arr[0], "wlan0:");
    GRV_TEST_EQUAL_STR(arr.arr[1], "0000");
    GRV_TEST_EQUAL_STR(arr.arr[2], "55.");
    GRV_TEST_EQUAL_STR(arr.arr[3], "-55.");
    GRV_TEST_EQUAL_STR(arr.arr[4], "-256");
    GRV_TEST_EQUAL_STR(arr.arr[5], "0");

GRV_TEST_END_NEW()

