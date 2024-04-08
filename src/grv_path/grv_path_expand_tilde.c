#include "grv/grv_path.h"
#include "grv/grv_log.h"
#include <stdlib.h>

grv_str_t grv_path_expand_tilde(grv_str_t path) {
    if (grv_str_starts_with_char(path, '~')) {
        char* home_path = getenv("HOME");
        if (home_path == NULL) {
            grv_log_error(grv_str_ref("HOME environmental variable not found."));
            assert(false);
        }
        return grv_str_cat(grv_str_ref(home_path), grv_str_substr(path, 1, -1));
    }
    return grv_str_copy(path);
}
