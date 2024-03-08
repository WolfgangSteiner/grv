#include <math.h>
#include "grv/grv_math.h"
#include "grv_math_sin_cos_lut.c"
#include <assert.h>

f32 unit_sin_f32(f32 arg) {
    return sinf(arg * TWO_PI_F32);
}

f32 unit_cos_f32(f32 arg) {
    return cosf(arg * TWO_PI_F32);
}

f32 sqr_f32(f32 a) { return a * a; }

f32 lerp_f32(f32 a, f32 b, f32 t) { return a + t * (b - a); }

f32 unit_sin_u16(u16 arg) {
    f32 rad = (f32)arg / 65536.0f * TWO_PI_F32;
    return sinf(rad);
}

f32 unit_cos_u16(u16 arg) {
    f32 rad = (f32)arg / 65536.0f * TWO_PI_F32;
    return cosf(rad);
}

#define SIN_COS_STEPS 4096
#define LUT_SIZE (SIN_COS_STEPS / 8)

f32 unit_sin_lut_u16(u16 arg) {
    sin_cos_t sc = unit_sin_cos_lut_u16(arg);
    return sc.sin;
}

f32 unit_cos_lut_u16(u16 arg) {
    sin_cos_t sc = unit_sin_cos_lut_u16(arg);
    return sc.cos;
}

sin_cos_t unit_sin_cos_u16(u16 arg) {
    return (sin_cos_t){unit_sin_u16(arg), unit_cos_u16(arg)};
}

sin_cos_t unit_sin_cos_lut_u16(u16 arg) {
    arg = arg & (SIN_COS_STEPS - 1);
    u32 sin_idx = 0;
    u32 cos_idx = 1;
    f32 sin_sign = 1.0f;
    f32 cos_sign = 1.0f;

    // shift down to lower half
    if (arg >= SIN_COS_STEPS / 2) {
        arg -= SIN_COS_STEPS / 2;
        sin_sign *= -1.0f;
        cos_sign *= -1.0f;
    }

    // shift to first quadrant
    if (arg >= SIN_COS_STEPS / 4) {
        arg -= SIN_COS_STEPS / 4;
        cos_sign *= -1.0f;
        sin_idx = 1;
        cos_idx = 0;
    }

    // reflect over pi/4
    if (arg > SIN_COS_STEPS / 8) {
        arg = SIN_COS_STEPS / 4 - arg;
        sin_idx = (sin_idx + 1) % 2;
        cos_idx = (cos_idx + 1) % 2;
    }
    assert(arg <= LUT_SIZE);
    float* sin_cos_val = unit_sin_cos_lut_4096[arg];
    return (sin_cos_t){sin_sign * sin_cos_val[sin_idx], cos_sign * sin_cos_val[cos_idx]};
}

i32 grv_count_leading_zero_bits_u8(u8 x) {
    i32 n = 0;
    if (x == 0) return 8;
    while (x > 0 && (x & 0x80) == 0) {
        n++;
        x <<= 1;
    }
    return n;
}

i32 grv_count_trailing_zero_bits_u8(u8 x) {
    i32 n = 0;
    if (x == 0) return 8;
    while (x > 0 && (x & 0x1) == 0) {
        n++;
        x >>= 1;
    }
    return n;
}
