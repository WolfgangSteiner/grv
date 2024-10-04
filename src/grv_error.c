#include "grv/grv_error.h"
#include "grv/grv_str.h"
#include "grv/grv_log.h"
#include <stdlib.h>
#include <errno.h>

char* _grv_error_file_not_readable_cstr = "Could not open file for reading";
char* _grv_error_file_not_writable_cstr = "Could not open file for writing";
char* _grv_error_file_no_capacity_cstr = "File system full while writing";

void grv_error_free(grv_error_t* err) {
    grv_str_free(&err->msg);
    err->type = GRV_ERROR_OTHER;
}

grv_error_t grv_error_create_msg_arg(grv_error_type_t type, grv_str_t msg, grv_str_t arg) {
    grv_str_t err_str = {0};
    grv_str_append_str(&err_str, msg);
    grv_str_append_cstr(&err_str, ": ");
    grv_str_append_str(&err_str, arg);
    grv_error_t res = { .success=false, type=type, .msg=err_str };
    return res;
}

void grv_error_log(grv_error_t err) {
    grv_log_error(err.msg);
}

void grv_abort(grv_error_t err) {
    grv_error_log(err);
    exit(1);
}

#define GRV_ERROR_TYPE_GET_STR_CASE(A) \
    case A: \
        return grv_str_ref(#A); \
        break; \


grv_str_t grv_error_type_get_str(grv_error_type_t type) {
    switch(type) {
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_OTHER)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_FILE_NOT_FOUND)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_FILE_NOT_READABLE)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_FILE_NOT_WRITABLE)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_FILE_NO_CAPACITY)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_FILE_EMPTY)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_FILE_MALFORMED)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_FILE_FORMAT_UNSUPPORTED)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_DIRECTORY_NOT_READABLE)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_PATH_EMPTY)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_PATH_EXISTS)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_CREATING_DIRECTORY)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_PERMISSION_DENIED)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_FILE_LOCKED)
    GRV_ERROR_TYPE_GET_STR_CASE(GRV_ERROR_INVALID_KEY)
    default:
        return grv_str_format(grv_str_ref("Unknown error code: {int}"), (int)type);
    }
}

grv_error_t grv_error_from_errno() {
    grv_error_t err = {0};
    err.success = false;
    switch(errno) {
    default:
        err.type = GRV_ERROR_OTHER;
    }
    return err;
}
     
        
