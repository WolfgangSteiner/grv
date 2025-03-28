#ifndef GRV_vec2_f64_H
#define GRV_vec2_f64_H

#include <math.h>
#include "grv_base.h"
#include "grv_common.h"

struct vec2i_s;

typedef struct {
    f64 x, y;
} vec2_f64;

GRV_INLINE vec2_f64 vec2_f64_make(f64 x, f64 y) { return (vec2_f64){.x=x, .y=y}; }
GRV_INLINE bool vec2_f64_eq(vec2_f64 a, vec2_f64 b) { return a.x == b.x && a.y == b.y; }
GRV_INLINE bool vec2_f64_is_zero(vec2_f64 a) { return a.x == 0.0 && a.y == 0.0; }
GRV_INLINE vec2_f64 vec2_f64_abs(vec2_f64 a) { return (vec2_f64){grv_abs_f64(a.x), grv_abs_f64(a.y)}; }
GRV_INLINE f64 vec2_f64_inner(vec2_f64 a, vec2_f64 b) { return a.x * b.x + a.y * b.y; }  
GRV_INLINE f64 vec2_f64_norm2_sq(vec2_f64 a) { return vec2_f64_inner(a, a); }  
GRV_INLINE f64 vec2_f64_norm2(vec2_f64 a) { return sqrtf(vec2_f64_norm2_sq(a)); }
GRV_INLINE f64 vec2_f64_max_element(vec2_f64 a) { return grv_max_f64(a.x, a.y); }
GRV_INLINE f64 vec2_f64_min_element(vec2_f64 a) { return grv_min_f64(a.x, a.y); }
GRV_INLINE vec2_f64 vec2_f64_add(vec2_f64 a, vec2_f64 b) { return (vec2_f64){a.x + b.x, a.y + b.y}; }
GRV_INLINE vec2_f64 vec2_f64_sub(vec2_f64 a, vec2_f64 b) { return (vec2_f64){a.x - b.x, a.y - b.y}; }
GRV_INLINE vec2_f64 vec2_f64_smul(vec2_f64 a, f64 b) { return (vec2_f64){a.x * b, a.y * b}; }
GRV_INLINE vec2_f64 vec2_f64_smula(vec2_f64 a, f64 b, vec2_f64 c) { return (vec2_f64){a.x*b+c.x, a.y*b+c.y}; }

GRV_INLINE vec2_f64 vec2_f64_clamp(vec2_f64 v, vec2_f64 a, vec2_f64 b) {
    return (vec2_f64){.x=grv_clamp_f64(v.x,a.x,b.x),.y=grv_clamp_f64(v.y,a.y,b.y)};
}
// rotate a vector, rot is fraction of unit circle
vec2_f64 vec2_f64_rot(vec2_f64 a, f64 rot);

// linear interpolation
vec2_f64 vec2_f64_lerp(vec2_f64 a, vec2_f64 b, f64 t);

// rounding operation
GRV_INLINE vec2i vec2_f64_round(vec2_f64 a) { return (vec2i){grv_round_f64(a.x), grv_round_f64(a.y)}; }

#endif
