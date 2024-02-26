#include "grv/grv_test.h"
#include "grv/grv_str.h"

GRV_TEST_BEGIN_NEW(grv_str_split_head_at_char)
    grv_str_t str = grv_str_ref("Hello World!");
    GRV_TEST_EQUAL_STR(grv_str_split_head_at_char(str, ' '), "Hello");
    GRV_TEST_EQUAL_STR(grv_str_split_head_at_char(str, 'H'), "");
    GRV_TEST_EQUAL_STR(grv_str_split_head_at_char(str, '!'), "Hello World");
    GRV_TEST_EQUAL_STR(grv_str_split_head_at_char(str, '*'), "Hello World!");
GRV_TEST_END_NEW()

