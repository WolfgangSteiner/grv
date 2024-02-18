#ifndef GRV_FS_H
#define GRV_FS_H

#include <stdio.h>
#include "grv_str.h"
#include "grv_strarr.h"

grv_str_t grv_fs_basename(grv_str_t path);
grv_str_t grv_fs_dirname(grv_str_t path);
grv_str_t grv_fs_stem(grv_str_t path);

// split a path into its components
grv_strarr_t grv_fs_split_path(grv_str_t path);

// join a path from its components
grv_str_t grv_fs_join_path(grv_strarr_t path_components); 

// read a file into a grv_str
grv_str_t grv_fs_read_file(grv_str_t path);

// get the size of a file
size_t grv_fs_file_size(FILE* file);

int grv_fs_file_mod_time(grv_str_t path);

bool grv_fs_is_file_newer_than(grv_str_t filename, grv_str_t other_filename);

grv_str_t grv_expand_tilde(grv_str_t path);

#ifdef _WIN32
char* grv_win_path_to_posix_path(wchar_t* win_path);
wchar_t* grv_posix_path_to_win_path(char* posix_path);
#endif

#endif
