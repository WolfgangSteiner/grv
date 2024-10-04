#ifndef GRV_IMG8_H
#define GRV_IMG8_H

#include "grv/grv_base.h"
#include "grv/grv_str_typedef.h"
#include "grv/grv_error.h"

typedef struct {
    i32 w,h,row_skip;
    u8* pixel_data;
    bool owns_data;
} grv_img8_t;

bool grv_img8_load_from_bmp(grv_str_t file_name, grv_img8_t* img, grv_error_t* error);
grv_img8_t grv_img8_from_str(grv_str_t input, i32 width, i32 height);

#endif
