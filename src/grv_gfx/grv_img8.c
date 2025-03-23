#include "grv_gfx/grv_img8.h"
#include "grv/grv_fs.h"
#include "grv/grv_str.h"
#include "grv/grv_memory.h"
#include "grv/grv_log.h"
#include <threads.h>

#pragma pack(push, 1)
typedef struct {
    u16 id;
    u32 file_size;
    u32 reserved;
    u32 data_offset;
} _grv_bmp_file_header_t;

typedef struct {
    u32 size;
    u32 w,h;
    u16 num_planes;
    u16 bits_per_pixel;
    u32 compression_type;
    u32 image_size;
    u32 x_ppm;
    u32 y_ppm;
    u32 num_colors_used;
    u32 num_important_colors;
} _grv_bmp_dib_header_t;

typedef struct {
    _grv_bmp_file_header_t file_header;
    _grv_bmp_dib_header_t dib_header;
    u8 data[];
} _grv_bmp_file_t;

#pragma pack(pop)

// ALTERNATIVES
// 1. fail on error and log msg
// 2. embed error code in img8
// 3. return grv_error_t*
// 4. return bool (success/fail) and accept grv_error_t* (which might be null)

bool grv_img8_load_from_bmp(grv_str_t file_name, grv_img8_t* img, grv_error_t* err) {
    bool success = false;
    grv_str_t data_str = {0};

    success = grv_str_read_from_file(file_name, &data_str, err);
    if (!success) {
        goto end;
    }

    _grv_bmp_file_t* bmp_data = (_grv_bmp_file_t*)data_str.data;
    size_t header_size = sizeof(_grv_bmp_file_header_t) + sizeof(_grv_bmp_dib_header_t);
    bool is_bmp_file = (data_str.size > header_size) && (data_str.data[0] == 'B' && data_str.data[1] == 'M');

    if (!is_bmp_file) {
        if (err) {
            err->type = GRV_ERROR_FILE_FORMAT_UNSUPPORTED;
            err->msg = grv_str_format(grv_str_ref("Not a bmp file: {str}"), file_name);
        }
        goto end;
    }

    _grv_bmp_file_header_t* file_header = &bmp_data->file_header;
    _grv_bmp_dib_header_t* dib_header = &bmp_data->dib_header;

    if (data_str.size < file_header->file_size) {
        if (err) {
            err->type = GRV_ERROR_FILE_MALFORMED;
            err->msg = grv_str_format(grv_str_ref("Malformed bmp file: {str}"), file_name);
        }
        goto end;
    }

    grv_log_info(grv_str_format(grv_str_ref("BPP: {int}"), (i32)dib_header->bits_per_pixel));
    grv_log_info(grv_str_format(grv_str_ref("BMP size: {int}x{int}"), dib_header->w, dib_header->h));
    u8* pixel_data = (u8*)bmp_data + file_header->data_offset;

    if (dib_header->compression_type != 0) {
        if (err) {
            err->type = GRV_ERROR_FILE_FORMAT_UNSUPPORTED;
            err->msg = grv_str_format(grv_str_ref("Compressed BMP files not support: {str}"), file_name);
        }
        goto end;
    } else {
        if (img->owns_data && img->pixel_data) {
            img->pixel_data = grv_realloc(img->pixel_data, dib_header->w * dib_header->h);
        } else {
            img->pixel_data = grv_alloc_zeros(dib_header->w * dib_header->h);
        }
        img->w = dib_header->w;
        img->h = dib_header->h;
        img->row_skip = img->w;
        img->owns_data = true;
        i32 bytes_per_row = img->w * dib_header->bits_per_pixel / 8;
        i32 bytes_per_row_padded = (bytes_per_row % 4) ? (bytes_per_row + 4) % 4 : bytes_per_row;
        u8* src_row_ptr = pixel_data;
        u8* dst_row_ptr = img->pixel_data + img->w * (img->h - 1);
        for (i32 y = 0; y < img->h; y++) {
            u8* src_pixel_ptr = src_row_ptr;
            u8* dst_pixel_ptr = dst_row_ptr;
            for (i32 x = 0; x < bytes_per_row; x++) {
                u8 src_pixel = *src_pixel_ptr++;
                if (dib_header->bits_per_pixel == 4) {
                    *dst_pixel_ptr++ = src_pixel >> 4;
                    *dst_pixel_ptr++ = src_pixel & 0xf;
                } else {
                    *dst_pixel_ptr++ = src_pixel;
                }
            }
            src_row_ptr += bytes_per_row_padded;
            dst_row_ptr -= img->w;
        }
    }
    success = true;
    err->success = true;

end:
    grv_str_free(&data_str);
    return err;
}

void _grv_img8_parse_str(u8* dst, grv_str_t input, i32 width, i32 height) {
    grv_assert(input.size >= width * height);
    char* src = input.data;
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            char c = *src++;
            c = (c == ' ') ? 0 : c - '0';
            *dst++ = c;
        }
    }
}

grv_img8_t grv_img8_from_str(grv_str_t input, i32 width, i32 height) {
    assert(width * height <= input.size);
    u8* pixel_data = grv_alloc(width * height);
    _grv_img8_parse_str(pixel_data, input, width, height);
    grv_img8_t img = {.w=width, .h=height, .row_skip=width, .pixel_data=pixel_data };
    return img;
}


