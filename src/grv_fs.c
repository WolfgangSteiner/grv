#include "grv_fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>

grv_str grv_fs_basename(grv_str* path) {
  grv_str result = {};
  char* path_cstr = grv_str_copy_cstr(path);
  char* basename_cstr = basename(path_cstr);
  if (basename_cstr) {
    result = grv_str_new(basename_cstr);
  }
  free(path_cstr);
  return result;
}

grv_str grv_fs_stem(grv_str* path) {
  if (grv_str_len(path) == 0 
      || grv_str_eq_cstr(path, ".")
      || grv_str_eq_cstr(path, "..")
      || grv_str_ends_with_cstr(path, "/")) {
    return grv_str_new("");
  } else {
    grv_strarr path_components = grv_fs_split_path(path);
    grv_str basename = grv_strarr_pop_back(&path_components);
    grv_str stem = grv_str_split_head_from_front(&basename, ".");
    grv_strarr_push(&path_components, &stem);
    grv_str result = grv_fs_join_path(&path_components);
    grv_str_free(&basename);
    grv_strarr_free(&path_components);
    return result;
  }
}

grv_strarr grv_fs_split_path(grv_str* path) {
  grv_strarr result = grv_str_split(path, "/");
  return result;
}

grv_str grv_fs_join_path(grv_strarr* path_components) {
  grv_str result = grv_strarr_join(path_components, "/");
  return result;
}

grv_str grv_fs_read_file(grv_str *path) {
  grv_str result = {};
  FILE *file = fopen(grv_str_cstr(path), "r");
  if (file) {
    size_t file_size = grv_fs_file_size(file);
    grv_str_init_with_capacity(&result, file_size);
    fread(grv_str_cstr(&result), 1, file_size, file);
    fclose(file);
    grv_str_set_size(&result, file_size);
    grv_str_add_null_terminator(&result);
  }
  return result;
}


size_t grv_fs_file_size(FILE* file) {
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return file_size;
}
