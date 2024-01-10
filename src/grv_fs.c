#include "grv/grv_fs.h"
#include "grv/grv_memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <sys/stat.h>
#endif

grv_str_t grv_fs_basename(grv_str_t path) {
  if (!grv_str_contains_char(path, '/')) return path;
  grv_str_iter_t find_iter = grv_str_rfind_char(&path, '/');
  return grv_str_substr(path, find_iter.pos + 1, path.size - find_iter.pos - 1);
}

grv_str_t grv_fs_dirname(grv_str_t path) {
  if (grv_str_eq_cstr(path, "/")) return grv_str_ref("/");
  if (grv_str_ends_with_char(path, '/')) path = grv_str_substr(path, 0, path.size - 1); 
  if (!grv_str_contains_char(path, '/')) return grv_str_ref(".");
  grv_str_iter_t find_iter = grv_str_rfind_char(&path, '/');
  return grv_str_substr(path, 0, find_iter.pos);
}

grv_str_t grv_fs_stem(grv_str_t path) {
  grv_str_t basename = grv_fs_basename(path);
  if (!grv_str_contains_char(basename, '.')) return path;
  grv_str_iter_t find_iter = grv_str_rfind_char(&basename, '.');
  grv_str_size_t offset = basename.data - path.data;
  return grv_str_substr(path, 0, find_iter.pos + offset);
}

grv_strarr_t grv_fs_split_path(grv_str_t path) {
  grv_strarr_t result = {0}; // = grv_str_split(path, "/");
  return result;
}

grv_str_t grv_fs_join_path(grv_strarr_t path_components) {
  grv_str_t result = grv_strarr_join(path_components, grv_str_ref("/"));
  return result;
}

grv_str_t grv_fs_read_file(grv_str_t path) {
  grv_str_t result = {0};
  FILE *file = fopen(grv_str_cstr(path), "r");
  if (file) {
    size_t file_size = grv_fs_file_size(file);
    grv_assert(file_size < GRV_MAX_VALUE(result.size));
    result = grv_str_new_with_capacity(file_size);
    fread(result.data, 1, file_size, file);
    fclose(file);
    result.size = (grv_str_size_t)file_size;
  }
  return result;
}

size_t grv_fs_file_size(FILE* file) {
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return file_size;
}

int grv_fs_file_mod_time(grv_str_t filename) {
    struct stat attr;
    char* filename_cstr = grv_str_cstr(filename);
    stat(filename_cstr, &attr);
    return attr.st_mtime;
}

bool grv_fs_is_file_newer_than(grv_str_t filename, grv_str_t other_filename) {
    int mod_time_a = grv_fs_file_mod_time(filename);
    int mod_time_b = grv_fs_file_mod_time(other_filename);
    return mod_time_a > mod_time_b;
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