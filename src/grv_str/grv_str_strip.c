#include "grv/grv_str.h"

#ifndef GRV_TEST_COMPILE
grv_str_t grv_str_strip(grv_str_t str) {
    return grv_str_rstrip(grv_str_lstrip(str));
}   

#else

#include "grv/grv_test.h"

GRV_TEST_BEGIN_NEW(grv_str_strip)
    grv_str_t s = grv_str_ref("   alpha beta gamma   ");
    s = grv_str_strip(s);
    GRV_TEST_EQUAL_STR(s, "alpha beta gamma");
    s = grv_str_ref(" \n  alpha beta gamma\n\r\t ");
    s = grv_str_strip(s);
    GRV_TEST_EQUAL_STR(s, "alpha beta gamma");
GRV_TEST_END_NEW()

#endif
