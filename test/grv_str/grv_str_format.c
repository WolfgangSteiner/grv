#include "grv/grv_str.h"
#include "grv/grv_test.h"

GRV_TEST_BEGIN_NEW(grv_str_format)
    grv_str_t s = grv_str_format(grv_str_ref("{int}"), 42);
    GRV_TEST_EQUAL_STR(s, "42");
    s = grv_str_format(grv_str_ref("{str} {str} {str}"), grv_str_ref("one"), grv_str_ref("two"), grv_str_ref("three"));
    GRV_TEST_EQUAL_STR(s, "one two three");
GRV_TEST_END_NEW()
