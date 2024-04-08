#include "grv/grv_path.h"

void grv_path_append(grv_str_t* path_a, grv_str_t path_b) {
    grv_str_append_char(path_a, GRV_PATH_SEPARATOR);
    grv_str_append_str(path_a, path_b);
}

