#ifndef GRV_VEC2_FIXED32_H
#define GRV_VEC2_FIXED32_H

#include "grv/grv_fixed32.h"

typedef struct {
    grv_fixed32_t x,y;
} grv_vec2_fixed32_t;


GRV_INLINE grv_vec2_fixed32_t grv_vec2_fixed32_from_i32(i32 x, i32 y) {
    return (grv_vec2_fixed32_t) {
        .x=grv_fixed32_from_i32(x),
        .y=grv_fixed32_from_i32(y)
    };
}

GRV_INLINE grv_vec2_fixed32_t grv_vec2_fixed32_from_f32(f32 x, f32 y) {
    return (grv_vec2_fixed32_t) {
        .x=grv_fixed32_from_f32(x),
        .y=grv_fixed32_from_f32(y)
    };
}

GRV_INLINE grv_vec2_fixed32_t grv_vec2_fixed32_add(grv_vec2_fixed32_t a, grv_vec2_fixed32_t b) {
    return (grv_vec2_fixed32_t) {
        .x=grv_fixed32_add(a.x, b.x),
        .y=grv_fixed32_add(a.y, b.y)
    };
}

GRV_INLINE grv_vec2_fixed32_t grv_vec2_fixed32_sub(grv_vec2_fixed32_t a, grv_vec2_fixed32_t b) {
    return (grv_vec2_fixed32_t) {
        .x=grv_fixed32_sub(a.x, b.x),
        .y=grv_fixed32_sub(a.y, b.y)
    };
}

GRV_INLINE grv_vec2_fixed32_t grv_vec2_fixed32_smul(grv_vec2_fixed32_t a, grv_fixed32_t b) {
    return (grv_vec2_fixed32_t) {
        .x=grv_fixed32_mul(a.x, b),
        .y=grv_fixed32_mul(a.y, b)
    };
}

GRV_INLINE grv_vec2_fixed32_t grv_vec2_fixed32_smula(
    grv_vec2_fixed32_t a,
    grv_fixed32_t b,
    grv_vec2_fixed32_t c) {
    return (grv_vec2_fixed32_t) {
        .x=grv_fixed32_add(grv_fixed32_mul(a.x, b), c.x),
        .y=grv_fixed32_add(grv_fixed32_mul(a.y, b), c.y)
    };
}

GRV_INLINE grv_vec2_fixed32_t grv_vec2_fixed32_clamp(
    grv_vec2_fixed32_t a,
    grv_vec2_fixed32_t b,
    grv_vec2_fixed32_t c) {
    return (grv_vec2_fixed32_t) {
        .x=grv_fixed32_clamp(a.x, b.x, c.x),
        .y=grv_fixed32_clamp(a.y, b.y, c.y)
    };
}

#endif
