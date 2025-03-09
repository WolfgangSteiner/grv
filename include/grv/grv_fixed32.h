#ifndef GRV_FIXED32_H
#define GRV_FIXED32_H

#include "grv/grv_base.h"


#define GRV_FIXED32_MUL 1024

typedef struct {
    i32 val;
} grv_fixed32_t;


GRV_INLINE grv_fixed32_t grv_fixed32_from_i32(i32 a) {
    return (grv_fixed32_t) { .val=a*GRV_FIXED32_MUL };
}

GRV_INLINE grv_fixed32_t grv_fixed32_from_f32(f32 a) {
    return (grv_fixed32_t) { .val=(i32)(a*((f32)GRV_FIXED32_MUL)) };
}

GRV_INLINE grv_fixed32_t grv_fixed32_add(grv_fixed32_t a, grv_fixed32_t b) {
    return (grv_fixed32_t) { .val=a.val + b.val };
}

GRV_INLINE grv_fixed32_t grv_fixed32_sub(grv_fixed32_t a, grv_fixed32_t b) {
    return (grv_fixed32_t) { .val=a.val - b.val };
}

GRV_INLINE grv_fixed32_t grv_fixed32_mul(grv_fixed32_t a, grv_fixed32_t b) {
    i64 acc = a.val;
    acc *= b.val;
    acc /= GRV_FIXED32_MUL;
    return (grv_fixed32_t) { .val=(i32)acc };
}

GRV_INLINE grv_fixed32_t grv_fixed32_mula(grv_fixed32_t a, grv_fixed32_t b, grv_fixed32_t c) {
    i64 acc = a.val;
    acc *= b.val;
    acc /= GRV_FIXED32_MUL;
    acc += c;
    return (grv_fixed32_t) { .val=(i32)acc };
}

GRV_INLINE grv_fixed32_t grv_fixed32_div(grv_fixed32_t a, grv_fixed32_t b) {
    i64 acc = a.val;
    acc *= GRV_FIXED32_MUL;
    acc /= b.val;
    return (grv_fixed32_t) { .val=(i32)acc };
}

GRV_INLINE i32 grv_fixed32_round(grv_fixed32_t a) {
    return (a.val + GRV_FIXED32_MUL / 2) / GRV_FIXED32_MUL;
}

GRV_INLINE grv_fixed32_t grv_fixed32_abs(grv_fixed32_t a) {
    return (grv_fixed32_t) { .val=(a.val < 0 ? -a.val : a.val) };
}

GRV_INLINE bool grv_fixed32_ge(grv_fixed32_t a, grv_fixed32_t b) {
    return a.val >= b.val;
}

GRV_INLINE bool grv_fixed32_gt(grv_fixed32_t a, grv_fixed32_t b) {
    return a.val > b.val;
}

GRV_INLINE bool grv_fixed32_le(grv_fixed32_t a, grv_fixed32_t b) {
    return a.val <= b.val;
}

GRV_INLINE bool grv_fixed32_lt(grv_fixed32_t a, grv_fixed32_t b) {
    return a.val < b.val;
}

GRV_INLINE grv_fixed32_t grv_fixed32_neg(grv_fixed32_t a) {
    return (grv_fixed32_t) { .val = -a.val };
}

#endif
