#include "grv/grv_test.h"
#include "grv/grv_path.h"

GRV_TEST_BEGIN_NEW(grv_path_split)
    grv_str_t path = grv_str_ref("/usr/bin/true");
    grv_strarr_t arr = grv_path_split(path);
    GRV_TEST_EQUAL_INT(arr.size, 4);
    GRV_TEST_EQUAL_STR(arr.arr[0], "/");
    GRV_TEST_EQUAL_STR(arr.arr[1], "usr");
    GRV_TEST_EQUAL_STR(arr.arr[2], "bin");
    GRV_TEST_EQUAL_STR(arr.arr[3], "true");
GRV_TEST_END_NEW()
