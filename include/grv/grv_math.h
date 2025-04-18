#ifndef GRV_MATH_H
#define GRV_MATH_H

#include "grv_base.h"
#include <math.h>

#define PI_F32  3.141592653589793f
#define TWO_PI_F32 (2.0f * PI_F32)
#define HALF_PI_F32 (PI_F32 / 2.0f)

typedef struct {
    f32 sin;
    f32 cos;
} sin_cos_t;


f32 unit_sin_f32(f32 arg);
f32 unit_cos_f32(f32 arg);

f32 unit_sin_u16(u16 arg);
f32 unit_cos_u16(u16 arg);

f32 unit_sin_lut_u16(u16 arg);
f32 unit_cos_lut_u16(u16 arg);

sin_cos_t unit_sin_cos_u16(u16 arg);
sin_cos_t unit_sin_cos_lut_u16(u16 arg);


f32 sqr_f32(f32 a);

f32 lerp_f32(f32 a, f32 b, f32 t);

i32 grv_count_leading_zero_bits_u8(u8 x);
i32 grv_count_trailing_zero_bits_u8(u8 x);

#endif
