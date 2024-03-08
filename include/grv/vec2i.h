#ifndef GRV_VEC2I_H
#define GRV_VEC2I_H

#include "grv_base.h"

typedef struct vec2i_s {
    i32 x, y;
} vec2i;


GRV_INLINE vec2i vec2i_add(vec2i a, vec2i b) { return (vec2i){.x=a.x+b.x, .y=a.y+b.y}; }
GRV_INLINE vec2i vec2i_sub(vec2i a, vec2i b) { return (vec2i){.x=a.x-b.x, .y=a.y-b.y}; }
GRV_INLINE void vec2i_swp(vec2i* a, vec2i* b) { vec2i t = *a; *a = *b; *b = t; }

#endif

