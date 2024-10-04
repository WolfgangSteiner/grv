#include "grv/grv_path.h"

grv_str_t grv_path_join(grv_strarr_t path_components) {
    grv_str_t path = {0};
    if (path_components.size == 0) return path;
    path = grv_str_copy(path_components.arr[0]);
    for (int i = 1; i < path_components.size; ++i) {
        grv_path_append(&path, path_components.arr[i]);
    }
    return path;
}

