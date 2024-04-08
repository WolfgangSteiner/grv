#include "grv/grv_test.h"
#include "grv/grv_path.h"

GRV_TEST_BEGIN_NEW(grv_path_join)
    grv_strarr_t arr = {0};
    grv_strarr_push_str_ref(&arr, "/");
    grv_strarr_push_str_ref(&arr, "usr");
    grv_strarr_push_str_ref(&arr, "bin");
    grv_strarr_push_str_ref(&arr, "true");
    grv_str_t path = grv_path_join(arr);
    GRV_TEST_EQUAL_STR(path, "/usr/bin/true");
    grv_strarr_pop_front(&arr);
    path = grv_path_join(arr);
    GRV_TEST_EQUAL_STR(path, "usr/bin/true");
GRV_TEST_END_NEW()
