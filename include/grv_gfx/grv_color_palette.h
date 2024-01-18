#ifndef COLOR_PALETTE_H
#define COLOR_PALETTE_H

#include "grv/grv_base.h"

typedef struct {
    u32 num_entries;
    u32* entries;
} color_palette_t;

typedef enum {
    GRV_COLOR_PALETTE_BW,
    GRV_COLOR_PALETTE_GRAY_4,
    GRV_COLOR_PALETTE_GRAY_8,
    GRV_COLOR_PALETTE_GRAY_16,
    GRV_COLOR_PALETTE_PICO8,
} grv_color_palette_type_t;


void color_palette_init(color_palette_t* palette, u32 num_entries);
void color_palette_init_grayscale(color_palette_t* palette, u32 num_entries);
void grv_color_palette_init_with_type(color_palette_t* palette, grv_color_palette_type_t type);
u32 color_palette_map(color_palette_t* palette, u8 index);
#endif