#include "grv/grv_path.h"

void grv_path_append(grv_str_t* path_a, grv_str_t path_b) {
    if (grv_str_empty(path_b)) return;

    if (!grv_str_eq_cstr(*path_a, GRV_PATH_SEPARATOR_CSTR) 
        && !grv_str_empty(*path_a)
        && !grv_str_ends_with_char(*path_a, GRV_PATH_SEPARATOR)
        && !grv_str_starts_with_char(*path_a, GRV_PATH_SEPARATOR)) {
        grv_str_append_char(path_a, GRV_PATH_SEPARATOR);
    }
    grv_str_append_str(path_a, path_b);
}

