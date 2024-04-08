#include "grv/grv_path.h"

grv_str_t grv_path_basename(grv_str_t path) {
  if (!grv_str_contains_char(path, GRV_PATH_SEPARATOR)) return path;
  grv_str_iter_t find_iter = grv_str_rfind_char(&path, GRV_PATH_SEPARATOR);
  return grv_str_substr(path, find_iter.pos + 1, path.size - find_iter.pos - 1);
}

