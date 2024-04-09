#include "grv/grv_str.h"

grv_str_t grv_str_remove_trailing_newline(grv_str_t str) {
    grv_str_t res = str;
    res.owns_data = false;
    while (res.size) {
        char last_char = res.data[res.size-1];
        if (last_char == '\n' || last_char == '\r') res.size--;
        else break;
    }
    return res;
}

