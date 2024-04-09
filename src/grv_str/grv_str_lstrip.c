#include "grv/grv_str.h"

#ifndef GRV_TEST_COMPILE

grv_str_t grv_str_lstrip(grv_str_t str) {
    grv_str_iter_t iter = grv_str_iter_begin(&str);
    while (!grv_str_iter_is_end(&iter) && grv_str_iter_is_whitespace(&iter)) {
        grv_str_iter_inc(&iter, 1);
    }
    return grv_str_substr_with_iters(iter, grv_str_iter_end(&str));
}

#else

#include "grv/grv_test.h"

GRV_TEST_BEGIN_NEW(grv_str_lstrip)
    grv_str_t s = grv_str_ref("  test  bla ");
    s = grv_str_lstrip(s);
    GRV_TEST_EQUAL_STR(s, "test  bla ");

    s = grv_str_ref("   ");
    s = grv_str_lstrip(s);
    GRV_TEST_TRUE(grv_str_empty(s)); 

    s = grv_str_ref("");
    s = grv_str_lstrip(s);
    GRV_TEST_TRUE(grv_str_empty(s)); 

    s = grv_str_ref("  \n\talpha beta gamma");
    s = grv_str_lstrip(s);
    GRV_TEST_EQUAL_STR(s, "alpha beta gamma");

GRV_TEST_END_NEW()

#endif
