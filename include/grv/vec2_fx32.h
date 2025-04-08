#ifndef GRV_VEC2_FX32_H
#define GRV_VEC2_FX32_H

#include "grv/fx32.h"

typedef struct {
    fx32 x,y;
} vec2_fx32;


GRV_INLINE vec2_fx32 vec2_fx32_from_i32(i32 x, i32 y) {
    return (vec2_fx32) {
        .x=fx32_from_i32(x),
        .y=fx32_from_i32(y)
    };
}

GRV_INLINE vec2_fx32 vec2_fx32_from_f32(f32 x, f32 y) {
    return (vec2_fx32) {
        .x=fx32_from_f32(x),
        .y=fx32_from_f32(y)
    };
}

GRV_INLINE vec2_fx32 vec2_fx32_from_vec2_f32(vec2f v) {
	return vec2_fx32_from_f32(v.x, v.y);
}

GRV_INLINE vec2_fx32 vec2_fx32_add(vec2_fx32 a, vec2_fx32 b) {
    return (vec2_fx32) {
        .x=fx32_add(a.x, b.x),
        .y=fx32_add(a.y, b.y)
    };
}

GRV_INLINE vec2_fx32 vec2_fx32_sub(vec2_fx32 a, vec2_fx32 b) {
    return (vec2_fx32) {
        .x=fx32_sub(a.x, b.x),
        .y=fx32_sub(a.y, b.y)
    };
}

GRV_INLINE vec2_fx32 vec2_fx32_smul(vec2_fx32 a, fx32 b) {
    return (vec2_fx32) {
        .x=fx32_mul(a.x, b),
        .y=fx32_mul(a.y, b)
    };
}

GRV_INLINE vec2_fx32 vec2_fx32_smula(
    vec2_fx32 a,
    fx32 b,
    vec2_fx32 c) {
    return (vec2_fx32) {
        .x=fx32_add(fx32_mul(a.x, b), c.x),
        .y=fx32_add(fx32_mul(a.y, b), c.y)
    };
}

GRV_INLINE vec2_fx32 vec2_fx32_clamp(
    vec2_fx32 a,
    vec2_fx32 b,
    vec2_fx32 c) {
    return (vec2_fx32) {
        .x=fx32_clamp(a.x, b.x, c.x),
        .y=fx32_clamp(a.y, b.y, c.y)
    };
}

GRV_INLINE vec2i vec2_fx32_round(vec2_fx32 a) {
    return (vec2i) {.x = fx32_round(a.x), .y = fx32_round(a.y)};
}

#endif
