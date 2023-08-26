#include "grv/grv_fs.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
  #include <windows.h>
#endif

grv_str_t grv_fs_basename(grv_str_t* path) {
  if (grv_str_len(path) == 0 
      || grv_str_eq_cstr(path, ".")
      || grv_str_eq_cstr(path, "..")
      || grv_str_ends_with_cstr(path, "/")) {
    return grv_str_new("");
  } else {
    grv_strarr_t path_components = grv_fs_split_path(path);
    grv_str_t basename = grv_strarr_pop_back(&path_components);
    grv_strarr_free(&path_components);
    return basename;
  }
}

grv_str_t grv_fs_stem(grv_str_t* path) {
  if (grv_str_len(path) == 0 
      || grv_str_eq_cstr(path, ".")
      || grv_str_eq_cstr(path, "..")
      || grv_str_ends_with_cstr(path, "/")) {
    return grv_str_new("");
  } else {
    grv_strarr_t path_components = grv_fs_split_path(path);
    grv_str_t basename = grv_strarr_pop_back(&path_components);
    grv_str_t stem = grv_str_split_head_from_front(&basename, ".");
    grv_strarr_push(&path_components, &stem);
    grv_str_t result = grv_fs_join_path(&path_components);
    grv_str_free(&basename);
    grv_strarr_free(&path_components);
    return result;
  }
}

grv_strarr_t grv_fs_split_path(grv_str_t* path) {
  grv_strarr_t result = grv_str_split(path, "/");
  return result;
}

grv_str_t grv_fs_join_path(grv_strarr_t* path_components) {
  grv_str_t result = grv_strarr_join(path_components, "/");
  return result;
}

grv_str_t grv_fs_read_file(grv_str_t *path) {
  grv_str_t result = {0};
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

#ifdef _WIN32
char* grv_win_path_to_posix_path(wchar_t* win_path) {
  size_t utf8_size = WideCharToMultiByte(CP_UTF8, 0, win_path, -1, NULL, 0, NULL, NULL);
  assert(utf8_size < INT_MAX);
  char* result = malloc(utf8_size);
  WideCharToMultiByte(CP_UTF8, 0, win_path, -1, result, (int)utf8_size, NULL, NULL);
  size_t i = 0;
  while (result[i] != '\0' && i < utf8_size) {
      if (result[i] == '\\') result[i] = '/';
      i++;
  }
  return result;
}

wchar_t* grv_posix_path_to_win_path(char* posix_path) {
  size_t utf16_size = MultiByteToWideChar(CP_UTF8, 0, posix_path, -1, NULL, 0);
  assert(utf16_size < INT_MAX);
  wchar_t* result = malloc(utf16_size * sizeof(wchar_t));
  MultiByteToWideChar(CP_UTF8, 0, posix_path, -1, result, (int)utf16_size);
  size_t i = 0;
  while (result[i] != '\0' && i < utf16_size) {
      if (result[i] == '/') result[i] = '\\';
      i++;
  }
  return result;
}
#endif