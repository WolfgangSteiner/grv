#include "grv/grv_str.h"

void grv_str_append_line(grv_str_t* str, grv_str_t append_str) {
    grv_str_append_char(str, '\n');
    grv_str_append_str(str, append_str);
}
