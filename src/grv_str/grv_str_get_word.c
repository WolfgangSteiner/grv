#include "grv/grv_str.h"

#ifndef GRV_TEST_COMPILE

static void _grv_str_iter_advance_to_word_start(grv_str_iter_t* iter) {
    while (!grv_str_iter_is_end(iter) && grv_str_iter_is_whitespace(iter)) grv_str_iter_inc(iter, 1);
}

static void _grv_str_iter_advance_to_word_end(grv_str_iter_t* iter) {
    while (!grv_str_iter_is_end(iter) && !grv_str_iter_is_whitespace(iter)) grv_str_iter_inc(iter, 1);
}

grv_str_t grv_str_get_word(grv_str_t str, i32 idx) {
    grv_str_t res = {0};
    if (idx < 0 || str.size == 0) return res;

    grv_str_iter_t start_iter = grv_str_iter_begin(&str);
    grv_str_iter_t end_iter = grv_str_iter_begin(&str);

    int num_words_found = 0;

    while (!grv_str_iter_is_end(&start_iter)) {
        _grv_str_iter_advance_to_word_start(&start_iter);
        if (grv_str_iter_is_end(&start_iter)) break;
        // found the start of a word
        num_words_found++;
        // initialize the end_iter
        end_iter = start_iter;
        _grv_str_iter_advance_to_word_end(&end_iter);

        if (idx == num_words_found - 1) {
            res = grv_str_substr_with_iters(start_iter, end_iter);
            break;
        }
        start_iter = end_iter;
    }

    return res;
}

#else
#include "grv/grv_test.h"

GRV_TEST_BEGIN_NEW(grv_str_get_word)
    grv_str_t s = grv_str_ref("  alpha beta   gamma  ");
    grv_str_t w = grv_str_get_word(s, 0);
    GRV_TEST_EQUAL_STR(w, "alpha");
    w = grv_str_get_word(s, 1);
    GRV_TEST_EQUAL_STR(w, "beta");
    w = grv_str_get_word(s, 2);
    GRV_TEST_EQUAL_STR(w, "gamma");
    w = grv_str_get_word(s, 3);
    GRV_TEST_TRUE(grv_str_empty(w));
GRV_TEST_END_NEW()

#endif
