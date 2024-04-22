#include "grv/grv_str.h"

#ifndef GRV_TEST_COMPILE
grv_str_t grv_str_rstrip(grv_str_t str) {
    grv_str_iter_t end_iter = grv_str_iter_rbegin(&str);
    while (!grv_str_iter_is_rend(&end_iter) && grv_str_iter_is_whitespace(&end_iter)) {
        grv_str_iter_dec(&end_iter, 1);
    }
    // increment iter by one to convert to forward iter
    grv_str_iter_inc(&end_iter, 1);
    return grv_str_substr_with_iters(grv_str_iter_begin(&str), end_iter);
}   

#else

#include "grv/grv_test.h"

GRV_TEST_BEGIN_NEW(grv_str_rstrip)
    grv_str_t s = grv_str_ref("alpha beta gamma   ");
    s = grv_str_rstrip(s);
    GRV_TEST_EQUAL_STR(s, "alpha beta gamma");
    s = grv_str_ref("   alpha beta gamma\n\r\t ");
    s = grv_str_rstrip(s);
    GRV_TEST_EQUAL_STR(s, "   alpha beta gamma");
GRV_TEST_END_NEW()

#endif
