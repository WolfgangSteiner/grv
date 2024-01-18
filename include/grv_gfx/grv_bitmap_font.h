#ifndef GRV_BITMAP_FONT_H
#define GRV_BITMAP_FONT_H

#include "grv_gfx/grv_frame_buffer.h"
#include "grv/vec2i.h"
#include "grv/grv_str.h"

typedef struct {
    u8* glyph_data;
    u32 glyph_count;
    u8 glyph_width;
    u8 glyph_height;
    s32 hskip;
    s32 vskip;
} grv_bitmap_font_t;

void grv_put_text_u8(grv_frame_buffer_t* fb, grv_str_t str, vec2i pos, grv_bitmap_font_t* font, u8 color);
vec2i grv_bitmap_font_calc_size(grv_bitmap_font_t* font, grv_str_t str);

extern grv_bitmap_font_t grv_basic_font_8x8;

#endif