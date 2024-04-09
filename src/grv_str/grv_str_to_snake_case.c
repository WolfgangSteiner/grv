#include "grv/grv_str.h"
#include "grv/grv_common.h"

grv_str_t grv_str_to_snake_case(grv_str_t str) {
    grv_str_t res = {0};
    char prev = 0;

    for (i32 i = 0; i < str.size; ++i) {
        char c = str.data[i];
        if (c == ' ') {
            grv_str_append_char(&res, '_');
        }
        else if (grv_char_is_lower(prev) && grv_char_is_upper(c)) {
            grv_str_append_char(&res, '_');
            grv_str_append_char(&res, grv_char_to_lower(c));
        } else {
            grv_str_append_char(&res, grv_char_to_lower(c));
        }
        prev = c;
    }
    return res;
}
