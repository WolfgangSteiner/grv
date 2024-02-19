#include "grv/grv_test.h"
#include "grv/grv_strarr.h"

GRV_TEST_BEGIN_NEW(grv_strarr_index_of_str)
    grv_strarr_t arr = grv_strarr_new();
    grv_strarr_push_str_ref(&arr, "alpha");
    grv_strarr_push_str_ref(&arr, "beta");
    grv_strarr_push_str_ref(&arr, "gamma");
    GRV_TEST_EQUAL_INT(grv_strarr_index_of_str(arr, grv_str_ref("alpha")), 0);
    GRV_TEST_EQUAL_INT(grv_strarr_index_of_str(arr, grv_str_ref("beta")), 1);
    GRV_TEST_EQUAL_INT(grv_strarr_index_of_str(arr, grv_str_ref("gamma")), 2);
    GRV_TEST_EQUAL_INT(grv_strarr_index_of_str(arr, grv_str_ref("delta")), -1);
GRV_TEST_END_NEW()
