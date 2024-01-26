#include "grv_gfx/grv_bitmap_font.h"
//#include "grv/gfxa.h"
#include "grv/grv_common.h"
#include "grv/grv_math.h"
#include <string.h>
#include "grv_basic_font_8x8_impl.c"

static void grv_put_char_u8(grv_frame_buffer_t* fb, char c, vec2i pos, grv_bitmap_font_t* font, s32 scale, u8 color) {
    u8* glyph_data = font->glyph_data + font->glyph_height * (u32)c;
    for (int y = 0; y < font->glyph_height; y++) {
        u8 glyph_row = *glyph_data++;
        for (int x = 0; x < 8; ++x) {
            bool has_pixel = (glyph_row >> x) & 0x1;
            if (has_pixel) {
                vec2i pixel_pos = vec2i_add(pos, (vec2i){scale*x, scale*y});
                grv_frame_buffer_set_pixel_scaled_u8(fb, pixel_pos, scale, color);
            }
        }
    }
}

u8* grv_bitmap_font_get_glyph_ptr(grv_bitmap_font_t* font, char c) {
    if (font == NULL) font = &grv_basic_font_8x8;
    return font->glyph_data + font->glyph_height * (u32)c;
}

vec2i grv_bitmap_font_glyph_size(grv_bitmap_font_t* font, char c) {
    if (font == NULL) font = &grv_basic_font_8x8;
    s32 y1 = 0; s32 y2 = font->glyph_height - 1;
    u8* glyph_ptr = grv_bitmap_font_get_glyph_ptr(font, c);
    while (glyph_ptr[y1] == 0 && y1 < font->glyph_height) y1++;
    if (y1 == font->glyph_height) return (vec2i){0};
    while (glyph_ptr[y2] == 0 && y2 >= 0) y2--;
    u8 or_row = 0;
    for (s32 y = y1; y <= y2; ++y) {
        u8 row = glyph_ptr[y];
        or_row |= row;
    }
    s32 w = font->glyph_width - grv_count_leading_zero_bits_u8(or_row) - grv_count_trailing_zero_bits_u8(or_row);
    s32 h = y2 - y1 + 1;
    return (vec2i){w, h};
}


void grv_put_text_u8(grv_frame_buffer_t* fb, grv_str_t str, vec2i pos, grv_bitmap_font_t* font, u8 color) {
    grv_put_text_scaled_u8(fb, str, pos, font, 1, color);
}

void grv_put_text_scaled_u8(grv_frame_buffer_t* fb, grv_str_t str, vec2i pos, grv_bitmap_font_t* font, s32 scale, u8 color) {
    vec2i line_pos = pos;
    vec2i glyph_pos = pos;
    if (font == NULL) font = &grv_basic_font_8x8;

    for (size_t i = 0; i < str.size; ++i) {
        char c = str.data[i];
        if (c == '\n') {
            line_pos.y += scale * font->vskip;
            glyph_pos = line_pos;
            continue;
        } else if (c == ' ') {
            glyph_pos.x += scale * font->hskip;
            continue;
        }
        grv_put_char_u8(fb, c, glyph_pos, font, scale, color);
        glyph_pos.x += scale * font->hskip;
    }
}

vec2i grv_bitmap_font_calc_size(grv_bitmap_font_t* font, grv_str_t text) {
    if (font == NULL) font = &grv_basic_font_8x8;
    s32 x = 0;
    vec2i size = {0};
    if (text.size == 0) return size;
    size.y = font->glyph_height;

    for (size_t i = 0; i < text.size; ++i) {
        char c = text.data[i];
        if (c == '\n') {
            size.y += font->vskip;
            x = 0;
        } else { 
            x += font->hskip;
            size.x = grv_max_s32(size.x, x);
        }
    }
    return size;
}

grv_bitmap_font_t* grv_get_cozette_font() {
    static grv_bitmap_font_t* cozette_font = NULL;
    if (cozette_font == NULL) {
        u32 size = (u32)&_binary_src_grv_gfx_cozette_psf_end - (u32)&_binary_src_grv_gfx_cozette_psf_start;
        cozette_font = grv_load_psf_font(&_binary_src_grv_gfx_cozette_psf_start, size);
    }
}
