#include "grv/grv_path.h"

grv_str_t grv_path_stem(grv_str_t path) {
  grv_str_t basename = grv_path_basename(path);
  if (!grv_str_contains_char(basename, '.')) return path;
  grv_str_iter_t find_iter = grv_str_rfind_char(&basename, '.');
  grv_str_size_t offset = basename.data - path.data;
  return grv_str_substr(path, 0, find_iter.pos + offset);
}
