#include "grv/grv_fs.h"
#include "grv/grv_base.h"
#include "grv/grv_error.h"
#include "grv/grv_memory.h"
#include "grv/grv_path.h"
#include "grv/grv_cstr.h"
#include "grv/grv_error.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <sys/stat.h>
  #include <dirent.h>
#endif

bool grv_fs_read_file(grv_str_t path, u8** data, size_t* size, grv_error_t* err) {
    char* path_cstr = grv_str_copy_to_cstr(path);
    FILE *file = fopen(path_cstr, "rb");
    grv_free(path_cstr);

    if (file == NULL) {
        if (err) {
            *err = grv_error_create_msg_arg(GRV_ERROR_FILE_NOT_READABLE, grv_str_ref("Could not open file for reading"), path);
        }
        goto end;
    } 
    //todo replace with fstat
    *size = grv_fs_file_size(file);
    *data = grv_alloc(*size);
    grv_assert(data);
    fread(*data, 1, *size, file);
    err->success = true;
end:
    if (file) fclose(file);
    return err;
}

size_t grv_fs_file_size(FILE* file) {
    grv_assert(file);
    //todo replace with fstat
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return file_size;
}

grv_u64_result_t grv_fs_file_mod_time(grv_str_t filename) {
    grv_u64_result_t res = {0};
    struct stat attr;
    char* filename_cstr = grv_str_copy_to_cstr(filename);
    int succ = stat(filename_cstr, &attr);
    grv_free(filename_cstr);
    
    if (succ == 0) {
        res.valid = true;
        res.value = attr.st_mtime;
    } else {
        res.valid = false;
        res.error = grv_error_from_errno();
    }
    return res;
}

grv_bool_result_t grv_fs_is_file_newer_than(grv_str_t filename, grv_str_t other_filename) {
    grv_bool_result_t res = {0};
    grv_u64_result_t res_a = grv_fs_file_mod_time(filename);
    if (!res_a.valid) {
        res.valid = false;
        res.error = res_a.error;
        return res;
    }
    grv_u64_result_t res_b = grv_fs_file_mod_time(other_filename);
    if (!res_b.valid) {
        res.valid = false;
        res.error = res_b.error;
        return res;
    }
    res.valid = true;
    res.value = (res_a.value > res_b.value);
    return res;
}

grv_error_t grv_read_directory(grv_str_t directory_path, grv_strarr_t* strarr) {
    grv_error_t err = {0};
    char* directory_path_cstr = grv_str_copy_to_cstr(directory_path);
    DIR* directory = opendir(directory_path_cstr);
    grv_free(directory_path_cstr);

    if (directory == NULL) {
        err = grv_error_create_msg_arg(GRV_ERROR_DIRECTORY_NOT_READABLE, grv_str_ref("Directory not readable"), directory_path);
        goto end;
    }

    struct dirent* entry;   
    while ((entry = readdir(directory)) != NULL) {
        if (grv_cstr_eq(entry->d_name, ".") || grv_cstr_eq(entry->d_name, "..")) continue;
        grv_str_t i_path = grv_str_new(entry->d_name);
        grv_path_prepend(&i_path, directory_path);
        grv_strarr_push(strarr, i_path);
    }
    err.success = true; 
end:
    if(directory) closedir(directory);
    return err;
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
    grv_error_t err = {0};
    if (status == 0) {
        err.success = true;
        return err;
    } else {
        grv_str_t msg = grv_str_ref("Could not remove file");
        switch (errno) {
            case ENOENT:
                return grv_error_create_msg_arg(GRV_ERROR_FILE_NOT_FOUND, msg, file_path);
            case EACCES:
            case EIO:
            case EPERM:
                return grv_error_create_msg_arg(GRV_ERROR_PERMISSION_DENIED, msg, file_path);
            case EBUSY:
                return grv_error_create_msg_arg(GRV_ERROR_FILE_LOCKED, msg, file_path);
            default:
                return grv_error_create_msg_arg(GRV_ERROR_OTHER, msg, file_path);
        }
    }
}

grv_error_t grv_make_directory(grv_str_t path, bool may_exist) {
    grv_error_t err = {0};
    char* path_cstr = grv_str_copy_to_cstr(path);
    int status = mkdir(path_cstr, 0755);
    grv_free(path_cstr);
    if (status == 0) {
        err.success = true;
    } else {
        if (errno == EEXIST) {
            if (may_exist) {
                err.success = true;
            } else {
                err = grv_error_create_msg_arg(GRV_ERROR_PATH_EXISTS, grv_str_ref("Directory already exists"), path);
            }
        } else if (errno == EACCES) {
                err = grv_error_create_msg_arg(GRV_ERROR_PERMISSION_DENIED, grv_str_ref("Permission denied creating directory"), path);
        } else {
                err = grv_error_create_msg_arg(GRV_ERROR_OTHER, grv_str_ref("Error creating directory"), path);
        }
    }
    return err;
}

grv_strarr_t _grv_make_path_generate_subpaths(grv_str_t path) {
    grv_str_t current_path = {0};
    if (grv_str_starts_with_char(path, GRV_PATH_SEPARATOR)) {
        current_path = grv_str_new("");
        path = grv_str_lstrip_char(path, GRV_PATH_SEPARATOR);
    } else {
        current_path = grv_str_new(".");
    }
    path = grv_str_rstrip_char(path, GRV_PATH_SEPARATOR);
    grv_strarr_t path_arr = {0};
    grv_strarr_t subpath_arr = grv_path_split(path);
    for (int i = 0; i < subpath_arr.size; ++i) {
        grv_path_append(&current_path, subpath_arr.arr[i]);
        grv_strarr_push_copy(&path_arr, current_path);
    }
    grv_strarr_free(&subpath_arr);
    return path_arr;
}

grv_error_t grv_make_path(grv_str_t path) {
    grv_error_t err = {0};
    if (grv_str_empty(path)) {
        err = grv_error_create_msg_arg(GRV_ERROR_PATH_EMPTY, grv_str_ref("Cannot create empty path"), path);
        goto end;
    }

    grv_strarr_t path_arr = _grv_make_path_generate_subpaths(path);

    path = grv_str_rstrip_char(path, GRV_PATH_SEPARATOR);
    if (grv_str_eq(path, ".") || grv_str_eq(path, "..")) {
        err = grv_error_create_msg_arg(GRV_ERROR_PATH_EXISTS, grv_str_ref("Cannot create path"), path);
        goto end;
    }

    for (int i = 0; i < path_arr.size; ++i) {
        grv_str_t current_path = path_arr.arr[i];
        if (!grv_path_exists(current_path)) {
            grv_error_t make_dir_err = grv_make_directory(current_path, true);
            if (!make_dir_err.success) {
                err = make_dir_err; 
                goto end;
            } 
        }
    }
    err.success = true;
end:
    grv_strarr_free(&path_arr);
    return err;
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
