#include "grv/grv_str.h"
#include "grv/grv_strarr.h"

struct grv_strarr_t grv_str_split_char(grv_str_t str, char sep) {
    grv_strarr_t arr = grv_strarr_new();
    grv_str_iter_t start_iter = grv_str_iter_begin(&str);
    grv_str_iter_t end_iter = start_iter;
    while (!grv_str_iter_is_end(&end_iter)) {
        grv_str_iter_match_up_to_char(&end_iter, sep);
        grv_strarr_push(&arr, grv_str_substr_with_iters(start_iter, end_iter));
        if (grv_str_iter_is_end(&end_iter)) break;
        end_iter.pos++;
        if (grv_str_iter_is_end(&end_iter)) grv_strarr_push(&arr, grv_str_ref(""));
        start_iter = end_iter; 
    }
    return arr;
}

