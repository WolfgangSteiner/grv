#ifndef GRV_FS_H
#define GRV_FS_H

#include <stdio.h>
#include "grv_str.h"
#include "grv_strarr.h"


// read a file into a grv_str
grv_str_t grv_fs_read_file(grv_str_t path);

// get the size of a file
size_t grv_fs_file_size(FILE* file);

int grv_fs_file_mod_time(grv_str_t path);

bool grv_fs_is_file_newer_than(grv_str_t filename, grv_str_t other_filename);

grv_error_t grv_remove_file(grv_str_t file_path);

typedef struct {
    grv_strarr_t entries;
    grv_error_t error;
} grv_read_directory_return_t;

grv_read_directory_return_t grv_read_directory(grv_str_t directory_path);

bool grv_path_exists(grv_str_t path);
grv_error_t grv_make_directory(grv_str_t path, bool may_exist);
grv_error_t grv_make_path(grv_str_t path);


#endif
