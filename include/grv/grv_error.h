#ifndef GRV_ERROR_H
#define GRV_ERROR_H

#include "grv/grv_str_typedef.h"

typedef enum {
    GRV_ERROR_OTHER=0,
    GRV_ERROR_FILE_NOT_FOUND,
    GRV_ERROR_FILE_NOT_READABLE,
    GRV_ERROR_FILE_NOT_WRITABLE,
    GRV_ERROR_FILE_NO_CAPACITY,
    GRV_ERROR_FILE_EMPTY,
    GRV_ERROR_FILE_MALFORMED,
    GRV_ERROR_FILE_FORMAT_UNSUPPORTED,
    GRV_ERROR_DIRECTORY_NOT_READABLE,
    GRV_ERROR_PATH_EMPTY,
    GRV_ERROR_PATH_EXISTS,
    GRV_ERROR_CREATING_DIRECTORY,
    GRV_ERROR_PERMISSION_DENIED,
    GRV_ERROR_FILE_LOCKED,
    GRV_ERROR_INVALID_KEY,
} grv_error_type_t;

typedef struct {
    bool success;
    grv_error_type_t type;
    grv_str_t msg;
} grv_error_t;

grv_error_t grv_error_create(grv_error_type_t type, grv_str_t msg);
grv_error_t grv_error_create_msg_arg(grv_error_type_t type, grv_str_t msg, grv_str_t arg);
grv_error_t grv_error_create_with_format(grv_error_type_t type, grv_str_t fmt_str, ...);
grv_error_t grv_error_from_errno(void);
void grv_error_free(grv_error_t*);
void grv_error_log(grv_error_t error);
void grv_abort(grv_error_t err);
grv_str_t grv_error_type_get_str(grv_error_type_t type);

extern char* _grv_error_file_not_readable_cstr;
extern char* _grv_error_file_not_writable_cstr;
extern char* _grv_error_file_no_capacity_cstr;

#endif
