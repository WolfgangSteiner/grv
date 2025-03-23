#ifndef GRV_VEC2F_H
#define GRV_VEC2F_H

#include <math.h>
#include "grv_base.h"
#include "grv_common.h"

struct vec2i_s;

typedef struct {
    f32 x, y;
} vec2f;

GRV_INLINE vec2f vec2f_make(f32 x, f32 y) { return (vec2f){.x=x, .y=y}; }
GRV_INLINE bool vec2f_eq(vec2f a, vec2f b) { return a.x == b.x && a.y == b.y; }
GRV_INLINE bool vec2f_is_zero(vec2f a) { return a.x == 0.0f && a.y == 0.0f; }
GRV_INLINE vec2f vec2f_abs(vec2f a) { return (vec2f){grv_abs_f32(a.x), grv_abs_f32(a.y)}; }
GRV_INLINE f32 vec2f_inner(vec2f a, vec2f b) { return a.x * b.x + a.y * b.y; }  
GRV_INLINE f32 vec2f_norm2_sq(vec2f a) { return vec2f_inner(a, a); }  
GRV_INLINE f32 vec2f_norm2(vec2f a) { return sqrtf(vec2f_norm2_sq(a)); }
GRV_INLINE f32 vec2f_max_element(vec2f a) { return grv_max_f32(a.x, a.y); }
GRV_INLINE f32 vec2f_min_element(vec2f a) { return grv_min_f32(a.x, a.y); }
GRV_INLINE vec2f vec2f_add(vec2f a, vec2f b) { return (vec2f){a.x + b.x, a.y + b.y}; }
GRV_INLINE vec2f vec2f_sub(vec2f a, vec2f b) { return (vec2f){a.x - b.x, a.y - b.y}; }
GRV_INLINE vec2f vec2f_smul(vec2f a, f32 b) { return (vec2f){a.x * b, a.y * b}; }
GRV_INLINE vec2f vec2f_smula(vec2f a, f32 b, vec2f c) { return (vec2f){a.x*b+c.x, a.y*b+c.y}; }

GRV_INLINE vec2f vec2f_clamp(vec2f v, vec2f a, vec2f b) {
    return (vec2f){.x=grv_clamp_f32(v.x,a.x,b.x),.y=grv_clamp_f32(v.y,a.y,b.y)};
}
// rotate a vector, rot is fraction of unit circle
vec2f vec2f_rot(vec2f a, f32 rot);

// linear interpolation
vec2f vec2f_lerp(vec2f a, vec2f b, f32 t);

// rounding operation
struct vec2i_s vec2f_round(vec2f a);


#endif
