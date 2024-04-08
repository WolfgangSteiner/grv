#include "grv/grv_path.h"

grv_str_t grv_path_dirname(grv_str_t path) {
  if (grv_str_eq_cstr(path, "/")) return grv_str_ref("/");
  if (grv_str_ends_with_char(path, GRV_PATH_SEPARATOR)) path = grv_str_substr(path, 0, path.size - 1); 
  if (!grv_str_contains_char(path, GRV_PATH_SEPARATOR)) return grv_str_ref(".");
  grv_str_iter_t find_iter = grv_str_rfind_char(&path, GRV_PATH_SEPARATOR);
  return grv_str_substr(path, 0, find_iter.pos);
}

