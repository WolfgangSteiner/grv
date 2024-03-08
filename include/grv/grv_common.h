#ifndef GRV_COMMON_H
#define GRV_COMMON_H

#include "grv_base.h"
#include <math.h>

GRV_INLINE i32 grv_round_f32(f32 a) { return (i32)roundf(a); }
GRV_INLINE i32 grv_floor_f32(f32 a) { return a < 0.0f ? (i32)a - 1 : (i32)a; }

GRV_INLINE i32 grv_min_i32(i32 a, i32 b) { return a < b ? a : b; }
GRV_INLINE i32 grv_max_i32(i32 a, i32 b) { return a > b ? a : b; }

GRV_INLINE u32 grv_min_u32(u32 a, u32 b) { return a < b ? a : b; }
GRV_INLINE u32 grv_max_u32(u32 a, u32 b) { return a > b ? a : b; }

GRV_INLINE i64 grv_min_i64(i64 a, i64 b) { return a < b ? a : b; }
GRV_INLINE i64 grv_max_i64(i64 a, i64 b) { return a > b ? a : b; }

GRV_INLINE u64 grv_min_u64(u64 a, u64 b) { return a < b ? a : b; }
GRV_INLINE u64 grv_max_u64(u64 a, u64 b) { return a > b ? a : b; }

GRV_INLINE size_t grv_min_size_t(size_t a, size_t b) { return a < b ? a : b; }
GRV_INLINE size_t grv_max_size_t(size_t a, size_t b) { return a > b ? a : b; }

GRV_INLINE f32 grv_min_f32(f32 a, f32 b) { return a < b ? a : b; }
GRV_INLINE f32 grv_max_f32(f32 a, f32 b) { return a > b ? a : b; }

GRV_INLINE i32 grv_clamp_i32(i32 x, i32 a, i32 b) { return x < a ? a : x > b ? b : x; }
GRV_INLINE f32 grv_clamp_f32(f32 x, f32 a, f32 b) { return x < a ? a : x > b ? b : x; }

GRV_INLINE bool grv_is_in_range_f32(f32 x, f32 a, f32 b) { return x >= a && x < b; }
GRV_INLINE bool grv_is_in_range_inc_f32(f32 x, f32 a, f32 b) { return x >= a && x <= b; }

u32 grv_dup_u8_u32(u8 a);
u64 grv_dup_u8_u64(u8 a);

GRV_INLINE f32 grv_abs_f32(f32 a) { return a < 0.0f ? -a : a; }
GRV_INLINE f32 grv_abs_i32(i32 a) { return a < 0 ? -a : a; }

GRV_INLINE void grv_swp_i32(i32* a, i32* b) { i32 c = *a; *a = *b; *b = c; }
GRV_INLINE void grv_swp_f32(f32* a, f32* b) { f32 c = *a; *a = *b; *b = c; }

#endif
