#include "grv/grv_test.h"
#include "grv/grv_str.h"

GRV_TEST_BEGIN_NEW(grv_str_remove_trailing_newline)
    grv_str_t a = grv_str_ref("This is a test\n\n");
    grv_str_t b = grv_str_remove_trailing_newline(a);
    grv_str_t c = grv_str_remove_trailing_newline(b);
    grv_str_t d = grv_str_remove_trailing_newline(grv_str_ref("This is a test\r\n"));
    GRV_TEST_EQUAL_STR(b, "This is a test");
    GRV_TEST_EQUAL_STR(c, "This is a test");
    GRV_TEST_EQUAL_STR(d, "This is a test");
GRV_TEST_END_NEW()
