#include "grv/grv_test.h"
#include "grv/grv.h"

GRV_TEST_BEGIN_NEW(grv_str_split_tail_from_front)
    grv_str_t str = grv_str_ref("This is a a test!");
    GRV_TEST_EQUAL_STR(grv_str_split_tail_from_front(str, grv_str_ref(" a ")), "a test!");
    GRV_TEST_EQUAL_STR(grv_str_split_tail_from_front(str, grv_str_ref("!")), "");
    GRV_TEST_EQUAL_STR(grv_str_split_tail_from_front(str, grv_str_ref("T")), "his is a a test!");
    GRV_TEST_EQUAL_STR(grv_str_split_tail_from_front(str, grv_str_ref("X")), "");
GRV_TEST_END_NEW()
