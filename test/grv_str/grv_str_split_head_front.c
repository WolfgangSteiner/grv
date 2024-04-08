#include "grv/grv_test.h"
#include "grv/grv_str.h"

GRV_TEST_BEGIN_NEW(grv_str_split_head_front)
    grv_str_t s = grv_str_ref("This is a test");
    grv_strpair_t res = grv_str_split_head_front(s, grv_str_ref(" "));
    GRV_TEST_EQUAL_STR(res.first, "This");
    GRV_TEST_EQUAL_STR(res.second, "is a test");
GRV_TEST_END_NEW()
