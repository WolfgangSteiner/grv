#include "grv/grv_fs.h"
#include "grv/grv_base.h"
#include "grv/grv_memory.h"
#include "grv/grv_str.h"
#include "grv/grv_cstr.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <sys/stat.h>
  #include <dirent.h>
#endif

grv_str_t grv_fs_basename(grv_str_t path) {
  if (!grv_str_contains_char(path, GRV_PATH_SEPARATOR)) return path;
  grv_str_iter_t find_iter = grv_str_rfind_char(&path, GRV_PATH_SEPARATOR);
  return grv_str_substr(path, find_iter.pos + 1, path.size - find_iter.pos - 1);
}

grv_str_t grv_fs_dirname(grv_str_t path) {
  if (grv_str_eq_cstr(path, "/")) return grv_str_ref("/");
  if (grv_str_ends_with_char(path, GRV_PATH_SEPARATOR)) path = grv_str_substr(path, 0, path.size - 1); 
  if (!grv_str_contains_char(path, GRV_PATH_SEPARATOR)) return grv_str_ref(".");
  grv_str_iter_t find_iter = grv_str_rfind_char(&path, GRV_PATH_SEPARATOR);
  return grv_str_substr(path, 0, find_iter.pos);
}

grv_str_t grv_fs_stem(grv_str_t path) {
  grv_str_t basename = grv_fs_basename(path);
  if (!grv_str_contains_char(basename, '.')) return path;
  grv_str_iter_t find_iter = grv_str_rfind_char(&basename, '.');
  grv_str_size_t offset = basename.data - path.data;
  return grv_str_substr(path, 0, find_iter.pos + offset);
}

grv_strarr_t grv_split_path(grv_str_t path) {
    grv_strarr_t result = {0};
    if (grv_str_empty(path)) return result;

    result = grv_str_split_char(path, GRV_PATH_SEPARATOR);
    if (grv_str_starts_with_char(path, GRV_PATH_SEPARATOR)) {
        *grv_strarr_front(result) = grv_str_ref(GRV_PATH_SEPARATOR_STR); 
    }
    return result;
}

grv_str_t grv_join_path(grv_strarr_t path_components) {
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

void grv_path_append(grv_str_t* path_a, grv_str_t path_b) {
    grv_str_append_char(path_a, GRV_PATH_SEPARATOR);
    grv_str_append_str(path_a, path_b);
}

void grv_path_prepend(grv_str_t* path, grv_str_t prefix_path) {
    grv_str_prepend_char(path, GRV_PATH_SEPARATOR);
    grv_str_prepend_str(path, prefix_path);
}

grv_str_t grv_fs_read_file(grv_str_t path) {
  grv_str_t result = {0};
  FILE *file = fopen(grv_str_cstr(path), "r");
  if (file) {
    size_t file_size = grv_fs_file_size(file);
    grv_assert(file_size < (size_t)GRV_MAX_VALUE(result.size));
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

grv_str_t grv_expand_tilde(grv_str_t path) {
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

grv_read_directory_return_t grv_read_directory(grv_str_t directory_path) {
    grv_read_directory_return_t res = {0};
    char* directory_path_cstr = grv_str_copy_to_cstr(directory_path);
    DIR* directory = opendir(directory_path_cstr);
    grv_free(directory_path_cstr);

    if (directory == NULL) {
        res.error = GRV_ERROR_DIRECTORY_NOT_READABLE;
        return res;
    }

    struct dirent* entry;   
    while ((entry = readdir(directory)) != NULL) {
        if (grv_cstr_eq(entry->d_name, ".") || grv_cstr_eq(entry->d_name, "..")) continue;
        grv_str_t i_path = grv_str_new(entry->d_name);
        grv_path_prepend(&i_path, directory_path);
        grv_strarr_push(&res.entries, i_path);
    }
    // Close the directory
    closedir(directory);
  
    return res;
}

bool grv_path_exists(grv_str_t path) {
    char* directory_path_cstr = grv_str_copy_to_cstr(path);
    DIR* directory = opendir(directory_path_cstr);
    grv_free(directory_path_cstr);
    if (directory) {
        closedir(directory);
        return true;
    }
    return false;
}

grv_error_t grv_remove_file(grv_str_t file_path) {
    int status = remove(grv_str_cstr(file_path));
    if (status == 0) {
        return GRV_ERROR_SUCCESS;
    } else {
        switch (errno) {
            case ENOENT:
                return GRV_ERROR_FILE_NOT_FOUND;
            case EACCES:
            case EIO:
            case EPERM:
                return GRV_ERROR_PERMISSION_DENIED;
            case EBUSY:
                return GRV_ERROR_FILE_LOCKED;
            default:
                return GRV_ERROR_OTHER;
        }
    }
}

grv_error_t grv_make_directory(grv_str_t path, bool may_exist) {
    char* path_cstr = grv_str_copy_to_cstr(path);
    int status = mkdir(path_cstr, 0755);
    grv_free(path_cstr);
    if (status == 0) {
        return GRV_ERROR_SUCCESS;
    } else {
        if (errno == EEXIST) {
            return may_exist ? GRV_ERROR_SUCCESS : GRV_ERROR_PATH_EXISTS;
        } else if (errno == EACCES) {
            return GRV_ERROR_PERMISSION_DENIED;
        } else {
            return GRV_ERROR_OTHER;
        }
    }
}

grv_error_t grv_make_path(grv_str_t path) {
    grv_error_t result = GRV_ERROR_SUCCESS;
    grv_str_t current_path = grv_str_new(".");
    grv_strarr_t path_arr = {0};

    if (grv_str_empty(path)) {
        result = GRV_ERROR_PATH_EMPTY;
        goto end;
    }
    path = grv_str_rstrip_char(path, GRV_PATH_SEPARATOR);
    if (grv_str_eq(path, ".") || grv_str_eq(path, "..")) {
        result = GRV_ERROR_PATH_EXISTS;
        goto end;
    }

    path_arr = grv_split_path(path);

    while (path_arr.size) {
        grv_path_append(&current_path, grv_strarr_pop_front(&path_arr));
        if (!grv_path_exists(current_path)) {
            grv_error_t make_dir_result = grv_make_directory(current_path, false);
            if (make_dir_result != GRV_ERROR_SUCCESS) {
                result = GRV_ERROR_CREATING_DIRECTORY;
                goto end;
            } 
        }
    }
end:
    grv_str_free(&current_path);
    grv_strarr_free(&path_arr);
    return result;
}


#ifdef _WIN32
char* grv_win_path_to_posix_path(wchar_t* win_path) {
  size_t utf8_size = WideCharToMultiByte(CP_UTF8, 0, win_path, -1, NULL, 0, NULL, NULL);
  assert(utf8_size < INT_MAX);
  char* result = malloc(utf8_size);
  WideCharToMultiByte(CP_UTF8, 0, win_path, -1, result, (int)utf8_size, NULL, NULL);
  size_t i = 0;
  while (result[i] != '\0' && i < utf8_size) {
      if (result[i] == '\\') result[i] = GRV_PATH_SEPARATOR;
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
      if (result[i] == GRV_PATH_SEPARATOR) result[i] = '\\';
      i++;
  }
  return result;
}
#endif
