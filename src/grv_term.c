#include "grv/grv_str.h"
#include <stdio.h>

void grv_print(grv_str_t str) {
    printf(grv_str_cstr(str));
    fflush(stdout);
}

void grv_println(grv_str_t str) {
    puts(grv_str_cstr(str));
}