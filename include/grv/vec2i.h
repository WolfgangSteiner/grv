#ifndef GRV_VEC2I_H
#define GRV_VEC2I_H

#include "grv_base.h"

typedef struct vec2i_s {
    s32 x, y;
} vec2i;


vec2i vec2i_add(vec2i a, vec2i b);
vec2i vec2i_sub(vec2i a, vec2i b);
void vec2i_swp(vec2i* a, vec2i* b);

#endif