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
    u8 glyph_byte_count;
    u8 hskip;
    u8 vskip;
    u8 em;
    u8 topskip;
    bool mirrored_definition;
} grv_bitmap_font_t;

void grv_put_text_u8(grv_frame_buffer_t* fb, grv_str_t str, vec2i pos, grv_bitmap_font_t* font, u8 color);
void grv_put_text_scaled_u8(grv_frame_buffer_t* fb, grv_str_t str, vec2i pos, grv_bitmap_font_t* font, s32 scale, u8 color);
vec2i grv_bitmap_font_calc_size(grv_bitmap_font_t* font, grv_str_t str);

extern grv_bitmap_font_t grv_basic_font_8x8;
extern char _binary_src_grv_gfx_cozette_psf_start[];
extern char _binary_src_grv_gfx_cozette_psf_end[];
grv_bitmap_font_t* grv_get_cozette_font();
grv_bitmap_font_t* grv_load_psf_font(char* data, size_t size);
#endif
