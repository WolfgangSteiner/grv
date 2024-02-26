#include "grv/grv_test.h"
#include "grv/grv_strarr.h"

GRV_TEST_BEGIN_NEW(grv_strarr_any_contains)
    grv_strarr_t arr = grv_str_split_whitespace(grv_str_ref("This is a test!"));
    GRV_TEST_TRUE(grv_strarr_any_contains_cstr(arr, "Thi"));
    GRV_TEST_FALSE(grv_strarr_any_contains_cstr(arr, "bla"));
GRV_TEST_END_NEW()
