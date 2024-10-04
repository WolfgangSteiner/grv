#include "grv/grv_fs.h"
#include "grv/grv_str.h"
#include "grv/grv_memory.h"
#include <stdio.h>

bool grv_str_read_from_file(grv_str_t file_name, grv_str_t* result_str, grv_error_t* err) {
    FILE* fp = fopen(grv_str_cstr(file_name), "rb");
    bool success = false;

    if (fp == 0) {
        if (err) {
            *err = grv_error_create_msg_arg(GRV_ERROR_FILE_NOT_READABLE, grv_str_ref(_grv_error_file_not_readable_cstr), file_name);
        }
        success = false;
        goto end;
    }

    size_t size = grv_fs_file_size(fp);
    *result_str = grv_str_new_with_capacity(size);

    size_t bytes_read = fread(result_str->data, sizeof(char), size, fp);
    assert(bytes_read == size);
    result_str->size = size;
    success = true;
    if (err) {
        err->success = true;
    }

end:
    if (fp) fclose(fp);
    return success;
}

