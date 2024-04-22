#include "grv/grv_str.h"

#ifndef GRV_TEST_COMPILE

grv_str_t grv_str_substr_with_iters(grv_str_iter_t start_iter, grv_str_iter_t end_iter) {
    grv_assert(start_iter.str == end_iter.str);
    grv_assert(start_iter.pos <= end_iter.pos);
    grv_str_t res = {
        .data=start_iter.str->data + start_iter.pos,
        .size=end_iter.pos - start_iter.pos,
        .is_valid=start_iter.str->is_valid,
        .owns_data=false
    };
    return res;
}

#else
#include "grv/grv_test.h"
GRV_TEST_BEGIN_NEW(grv_str_substr_with_iters)
    grv_str_t sa = grv_str_ref("alpha beta gamma");
    grv_str_t sb = grv_str_substr_with_iters(grv_str_iter_begin(&sa), grv_str_iter_end(&sa));
    GRV_TEST_EQUAL_STR(sb, sa);
GRV_TEST_END_NEW()

#endif
