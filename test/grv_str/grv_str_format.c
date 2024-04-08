#include "grv/grv_str.h"
#include "grv/grv_test.h"
#include "grv/grv.h"

GRV_TEST_BEGIN_NEW(grv_str_format)
    grv_str_t s = grv_str_format(grv_str_ref("{int}"), 42);
    GRV_TEST_EQUAL_STR(s, "42");
GRV_TEST_END_NEW()
