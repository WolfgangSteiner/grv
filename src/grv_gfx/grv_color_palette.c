#include "grv_gfx/grv_color_palette.h"
#include "grv_gfx/grv_color.h"
#include "grv/grv_memory.h"
#include <stdlib.h>

void color_palette_init(color_palette_t* palette, u32 num_entries) {
    palette->num_entries = num_entries;
    palette->entries = grv_alloc_zeros(num_entries * sizeof(u32));
}

void color_palette_init_grayscale(color_palette_t* palette, u32 num_entries) {
    color_palette_init(palette, num_entries);
    for(u32 i = 0; i < num_entries; i++) {
        u8 val = (u8)((f32)i / (f32)(num_entries - 1) * 255.0f);
        palette->entries[i] = grv_pack_rgba(val, val, val, 255);
    }
}

u32 color_palette_map(color_palette_t* palette, u8 index) {
    return palette->entries[index % palette->num_entries];
}

void grv_color_palette_init_with_type(color_palette_t* palette, grv_color_palette_type_t type) {
    switch(type) {
        case GRV_COLOR_PALETTE_BW: {
            color_palette_init(palette, 2);
            palette->entries[0] = 0x000000FF;
            palette->entries[1] = 0xFFFFFFFF;
        } break;
        case GRV_COLOR_PALETTE_GRAY_4: {
            color_palette_init_grayscale(palette, 4);
        } break;
        case GRV_COLOR_PALETTE_GRAY_8: {
            color_palette_init_grayscale(palette, 8);
        } break;
        case GRV_COLOR_PALETTE_GRAY_16: {
            color_palette_init_grayscale(palette, 16);
        } break;
        case GRV_COLOR_PALETTE_PICO8: {
            color_palette_init(palette, 16);
            palette->entries[0] = 0x000000FF;
            palette->entries[1] = 0x1D2B53FF;
            palette->entries[2] = 0x7E2553FF;
            palette->entries[3] = 0x008751FF;
            palette->entries[4] = 0xAB5236FF;
            palette->entries[5] = 0x5F574FFF;
            palette->entries[6] = 0xC2C3C7FF;
            palette->entries[7] = 0xFFF1E8FF;
            palette->entries[8] = 0xFF004DFF;
            palette->entries[9] = 0xFFA300FF;
            palette->entries[10] = 0xFFEC27FF;
            palette->entries[11] = 0x00E436FF;
            palette->entries[12] = 0x29ADFFFF;
            palette->entries[13] = 0x83769CFF;
            palette->entries[14] = 0xFF77A8FF;
            palette->entries[15] = 0xFFCCAAFF;
        } break;
    }
}