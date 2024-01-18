#ifndef GRV_COLOR_H
#define GRV_COLOR_H
#include "grv/grv_base.h"

GRV_INLINE u32 grv_pack_rgba(u8 r, u8 g, u8 b, u8 a) {
    return (u32)r << 24 | (u32)g << 16 | (u32)b << 8 | a;
}

GRV_INLINE u32 grv_rgba_to_argb(u32 rgba) {
    return (rgba >> 8) | (rgba << 24);
}

#endif