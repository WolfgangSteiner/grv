#include "grv/grv_path.h"

void grv_path_prepend(grv_str_t* path, grv_str_t prefix_path) {
    grv_str_prepend_char(path, GRV_PATH_SEPARATOR);
    grv_str_prepend_str(path, prefix_path);
}

