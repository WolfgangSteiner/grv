#ifndef GRV_STR_TYPES_H
#define GRV_STR_TYPES_H
#include "grv_base.h"

typedef i32 grv_str_size_t;

typedef struct {
    char* data;
    grv_str_size_t size;
    bool is_valid;
    bool owns_data;
} grv_str_t;

typedef struct {
    grv_str_t* str;
    grv_str_size_t pos;
} grv_str_iter_t;

#endif
