#include "grv/grv_test.h"
#include "grv/grv_cstr.h"

GRV_TEST_BEGIN_NEW(grv_cstr_len)
    GRV_TEST_EQUAL_INT(grv_cstr_len("Hello World!"), 12);
    GRV_TEST_EQUAL_INT(grv_cstr_len(grv_cstr_new("Hello World!")), 12);
GRV_TEST_END_NEW()
