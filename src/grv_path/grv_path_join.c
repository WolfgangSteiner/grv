#include "grv/grv_path.h"

grv_str_t grv_path_join(grv_strarr_t path_components) {
    grv_str_t path = {0};
    if (path_components.size == 0) return path;
    i32 idx = 0;
    if (grv_str_eq_cstr(path_components.arr[0], GRV_PATH_SEPARATOR_STR)) {
        path = grv_str_ref(GRV_PATH_SEPARATOR_STR);
        idx++;
    }

    bool first = true;

    while (idx < path_components.size) {
        if (!first) grv_str_append_char(&path, GRV_PATH_SEPARATOR);
        grv_str_append_str(&path, path_components.arr[idx]);
        idx++;
        first = false;
    }
    
    return path;
}

