#include "grv_gfx/grv_bitmap_font.h"
//#include "grv/gfxa.h"
#include "grv/grv_base.h"
#include "grv/grv_common.h"
#include "grv/grv_math.h"
#include "grv/grv_log.h"
#include <string.h>
#include "grv_basic_font_8x8_impl.c"

u8 _grv_bitmap_font_invert_byte(u8 byte) {
    byte = (byte >> 4) | (byte << 4);
    byte = ((byte >> 2) & 0x33) | ((byte << 2) & 0xcc);
    byte = ((byte >> 1) & 0x55) | ((byte << 1) & 0xaa);
    return byte;
}

static void grv_put_char_u8(grv_frame_buffer_t* fb, char c, vec2i pos, grv_bitmap_font_t* font, i32 scale, u8 color) {
    u8* glyph_data = font->glyph_data + font->glyph_byte_count * (u32)c;
    u8 bytes_per_row = font->glyph_byte_count / font->glyph_height;
    u8* glyph_row_ptr = glyph_data + font->topskip * bytes_per_row;
    for (int y = 0; y < font->glyph_height; y++) {
        for (int byte_count = 0; byte_count < bytes_per_row; byte_count++) {
            u8 current_byte = *glyph_row_ptr++;
            if (font->mirrored_definition) current_byte = _grv_bitmap_font_invert_byte(current_byte);
            for (int x = 0; x < 8; ++x) {
                bool has_pixel = (current_byte << x) & 0x80;
                if (has_pixel) {
                    vec2i pixel_pos = vec2i_add(pos, (vec2i){scale*x, scale*y});
                    grv_frame_buffer_set_pixel_scaled_u8(fb, pixel_pos, scale, color);
                }
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
    i32 y1 = 0; i32 y2 = font->glyph_height - 1;
    u8* glyph_ptr = grv_bitmap_font_get_glyph_ptr(font, c);
    while (glyph_ptr[y1] == 0 && y1 < font->glyph_height) y1++;
    if (y1 == font->glyph_height) return (vec2i){0};
    while (glyph_ptr[y2] == 0 && y2 >= 0) y2--;
    u8 or_row = 0;
    for (i32 y = y1; y <= y2; ++y) {
        u8 row = glyph_ptr[y];
        or_row |= row;
    }
    i32 w = font->glyph_width - grv_count_leading_zero_bits_u8(or_row) - grv_count_trailing_zero_bits_u8(or_row);
    i32 h = y2 - y1 + 1;
    return (vec2i){w, h};
}


void grv_put_text_u8(grv_frame_buffer_t* fb, grv_str_t str, vec2i pos, grv_bitmap_font_t* font, u8 color) {
    grv_put_text_scaled_u8(fb, str, pos, font, 1, color);
    //grv_put_text_scaled_u8(fb, str, (vec2i){pos.x+1, pos.y}, font, 1, color);
}

void grv_put_text_scaled_u8(grv_frame_buffer_t* fb, grv_str_t str, vec2i pos, grv_bitmap_font_t* font, i32 scale, u8 color) {
    vec2i line_pos = pos;
    vec2i glyph_pos = pos;
    if (font == NULL) font = &grv_basic_font_8x8;

    for (grv_str_size_t i = 0; i < str.size; ++i) {
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
    i32 x = 0;
    vec2i size = {0};
    if (text.size == 0) return size;
    size.y = font->glyph_height;
    i32 max_chars_per_line = 0;
    i32 font_spacing = font->hskip - font->em;

    for (grv_str_size_t i = 0; i < text.size; ++i) {
        char c = text.data[i];
        if (c == '\n') {
            size.y += font->vskip;
            x = 0;
        } else { 
            x++;
            max_chars_per_line = grv_max_i32(max_chars_per_line, x);
        }
    }
    size.x = max_chars_per_line * font->em + (max_chars_per_line - 1) * font_spacing;
    return size;
}

struct psf_header {
    u32 magic;
    u32 version;
    u32 header_size;
    u32 flags;
    u32 glyph_count;
    u32 glyph_byte_count;
    u32 glyph_height;
    u32 glyph_width;
};

#if 0
static void _grv_psf_font_extract_glyph_geometry(grv_bitmap_font_t* font) {
    u8* glyph_data = font->glyph_data + font->glyph_byte_count * 'A';
    GRV_UNUSED(glyph_data);
}
#endif 

grv_bitmap_font_t* grv_load_psf_font(char* data, size_t size) {
    GRV_UNUSED(size);
    grv_bitmap_font_t* font = grv_alloc_zeros(sizeof(grv_bitmap_font_t));
    //grv_bitmap_font_t* font = grv_alloc_zeros(sizeof(grv_bitmap_font_t));
    u32 magic_bytes = *(u32*)data;
    assert(magic_bytes == 0x864ab572);
    struct psf_header* header = (struct psf_header*)data;
    font->glyph_width = header->glyph_width;
    font->glyph_height = header->glyph_height;
    font->glyph_count = header->glyph_count;
    font->glyph_byte_count = header->glyph_byte_count;
    font->hskip = header->glyph_width;
    font->vskip = header->glyph_height;
    font->glyph_data = (u8*)data + header->header_size;
    return font;
}

grv_bitmap_font_t* grv_get_cozette_font(void) {
    static grv_bitmap_font_t* cozette_font = NULL;
    if (cozette_font == NULL) {
        grv_log_info(grv_str_ref("Loading cozette font"));
        size_t size = (size_t)&_binary_src_grv_gfx_cozette_psf_end - (size_t)&_binary_src_grv_gfx_cozette_psf_start;
        cozette_font = grv_load_psf_font((char*)&_binary_src_grv_gfx_cozette_psf_start, size);
        cozette_font->hskip = 7;
        cozette_font->vskip = 9;
        cozette_font->em = 5;
        cozette_font->topskip = 2;
    }
    return cozette_font;
}
