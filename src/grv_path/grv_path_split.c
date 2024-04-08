#include "grv/grv_path.h"

grv_strarr_t grv_path_split(grv_str_t path) {
    grv_strarr_t result = {0};
    if (grv_str_empty(path)) return result;

    result = grv_str_split_char(path, GRV_PATH_SEPARATOR);
    if (grv_str_starts_with_char(path, GRV_PATH_SEPARATOR)) {
        *grv_strarr_front(result) = grv_str_ref(GRV_PATH_SEPARATOR_STR); 
    }
    return result;
}
