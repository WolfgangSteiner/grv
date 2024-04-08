#include "grv/grv_str.h"

grv_strpair_t grv_str_split_head_front(grv_str_t str, grv_str_t sep) {
    grv_strpair_t res = {0};
    grv_str_iter_t start_iter = grv_str_iter_begin(&str);
    grv_str_iter_t end_iter = grv_str_find_str(&str, sep);
    res.first = grv_str_substr_with_iters(start_iter, end_iter);
    if (!grv_str_iter_is_end(&end_iter)) {
        start_iter = end_iter;
        grv_str_iter_match_str(&start_iter, sep);
        end_iter = grv_str_iter_end(&str);
        res.second = grv_str_substr_with_iters(start_iter, end_iter);
    }
    return res;
}

