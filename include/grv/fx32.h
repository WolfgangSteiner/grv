#ifndef GRV_FIXED32_H
#define GRV_FIXED32_H

#include "grv/grv_base.h"


#define GRV_FIXED32_MUL (1 << 10)

typedef struct {
    i32 val;
} fx32;


GRV_INLINE fx32 fx32_from_i32(i32 a) {
    return (fx32) { .val=a*GRV_FIXED32_MUL };
}

GRV_INLINE fx32 fx32_from_f32(f32 a) {
    return (fx32) { .val=(i32)(a*((f32)GRV_FIXED32_MUL)) };
}

GRV_INLINE fx32 fx32_from_f64(f64 a) {
    return (fx32) { .val=(i32)(a*((f64)GRV_FIXED32_MUL)) };
}

GRV_INLINE f64 fx32_to_f64(fx32 a) {
    return a.val / (f64)GRV_FIXED32_MUL;
}

GRV_INLINE fx32 fx32_add(fx32 a, fx32 b) {
    return (fx32) { .val=a.val + b.val };
}

GRV_INLINE fx32 fx32_sub(fx32 a, fx32 b) {
    return (fx32) { .val=a.val - b.val };
}

GRV_INLINE fx32 fx32_mul(fx32 a, fx32 b) {
    i64 acc = a.val;
    acc *= b.val;
    acc /= GRV_FIXED32_MUL;
    return (fx32) { .val=(i32)acc };
}

GRV_INLINE fx32 fx32_mula(fx32 a, fx32 b, fx32 c) {
    i64 acc = a.val;
    acc *= b.val;
    acc /= GRV_FIXED32_MUL;
    acc += c.val;
    return (fx32) { .val=(i32)acc };
}

GRV_INLINE fx32 fx32_div(fx32 a, fx32 b) {
    i64 acc = a.val;
    acc *= GRV_FIXED32_MUL;
    acc /= b.val;
    return (fx32) { .val=(i32)acc };
}

GRV_INLINE fx32 fx32_inv(fx32 a) {
	grv_assert(a.val != 0);
	return fx32_div(fx32_from_i32(1), a);
}

GRV_INLINE i32 fx32_round(fx32 a) {
    i32 acc = a.val;
    acc += (a.val > 0 ? GRV_FIXED32_MUL / 2 : -GRV_FIXED32_MUL / 2);
    acc /= GRV_FIXED32_MUL;
    return acc;
}

GRV_INLINE f32 fx32o_f32(fx32 a) {
    return (f32)a.val / (f32)GRV_FIXED32_MUL;
}

GRV_INLINE fx32 fx32_abs(fx32 a) {
    return (fx32) { .val=(a.val < 0 ? -a.val : a.val) };
}

GRV_INLINE bool fx32_ge(fx32 a, fx32 b) {
    return a.val >= b.val;
}

GRV_INLINE bool fx32_gt(fx32 a, fx32 b) {
    return a.val > b.val;
}

GRV_INLINE bool fx32_le(fx32 a, fx32 b) {
    return a.val <= b.val;
}

GRV_INLINE bool fx32_lt(fx32 a, fx32 b) {
    return a.val < b.val;
}

GRV_INLINE fx32 fx32_neg(fx32 a) {
    return (fx32) { .val = -a.val };
}

GRV_INLINE fx32 fx32_min(fx32 a, fx32 b) {
    i32 val = a.val < b.val ? a.val : b.val;
    return (fx32) { .val=val };
}

GRV_INLINE fx32 fx32_max(fx32 a, fx32 b) {
    i32 val = a.val > b.val ? a.val : b.val;
    return (fx32) { .val=val };
}

GRV_INLINE fx32 fx32_clamp(fx32 a, fx32 b, fx32 c) {
    i32 val = a.val < b.val ? b.val : a.val > c.val ? c.val : a.val;
    return (fx32) { .val=val };
}

GRV_INLINE fx32 fx32_add_i32(fx32 a, i32 b) {
	return fx32_add(a, fx32_from_i32(b));
}

GRV_INLINE fx32 fx32_mul_i32(fx32 a, i32 b) {
	return (fx32) {.val=a.val*b};
}

GRV_INLINE fx32 fx32_mula_i32(fx32 a, i32 b, i32 c) {
	return (fx32) {.val=a.val*b + c * GRV_FIXED32_MUL};
}

GRV_INLINE fx32 fx32_div_i32(fx32 a, i32 b) {
	return (fx32) {.val=a.val/b};
}

GRV_INLINE fx32 fx32_mul_f64(fx32 a, f64 b) {
	return fx32_mul(a, fx32_from_f64(b));
}

GRV_INLINE fx32 fx32_div_f64(fx32 a, f64 b) {
	return fx32_div(a, fx32_from_f64(b));
}

GRV_INLINE fx32 fx32_add_f64(fx32 a, f64 b) {
	return fx32_add(a, fx32_from_f64(b));
}

GRV_INLINE fx32 fx32_mula_f64(fx32 a, f64 b, f64 c) {
	return fx32_add_f64(fx32_mul_f64(a, b), c);
}

#endif
