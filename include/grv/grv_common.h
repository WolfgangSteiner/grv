#ifndef GRV_COMMON_H
#define GRV_COMMON_H

#include "grv_base.h"
#include <math.h>

GRV_INLINE s32 grv_round_f32(f32 a) { return (s32)roundf(a); }
GRV_INLINE s32 grv_floor_f32(f32 a) { return a < 0.0f ? (s32)a - 1 : (s32)a; }

GRV_INLINE s32 grv_min_s32(s32 a, s32 b) { return a < b ? a : b; }
GRV_INLINE s32 grv_max_s32(s32 a, s32 b) { return a > b ? a : b; }

GRV_INLINE u32 grv_min_u32(u32 a, u32 b) { return a < b ? a : b; }
GRV_INLINE u32 grv_max_u32(u32 a, u32 b) { return a > b ? a : b; }

GRV_INLINE s64 grv_min_s64(s64 a, s64 b) { return a < b ? a : b; }
GRV_INLINE s64 grv_max_s64(s64 a, s64 b) { return a > b ? a : b; }

GRV_INLINE u64 grv_min_u64(u64 a, u64 b) { return a < b ? a : b; }
GRV_INLINE u64 grv_max_u64(u64 a, u64 b) { return a > b ? a : b; }

GRV_INLINE size_t grv_min_size_t(size_t a, size_t b) { return a < b ? a : b; }
GRV_INLINE size_t grv_max_size_t(size_t a, size_t b) { return a > b ? a : b; }

GRV_INLINE f32 grv_min_f32(f32 a, f32 b) { return a < b ? a : b; }
GRV_INLINE f32 grv_max_f32(f32 a, f32 b) { return a > b ? a : b; }

GRV_INLINE s32 grv_clamp_s32(s32 x, s32 a, s32 b) { return x < a ? a : x > b ? b : x; }
GRV_INLINE f32 grv_clamp_f32(f32 x, f32 a, f32 b) { return x < a ? a : x > b ? b : x; }

GRV_INLINE bool grv_is_in_range_f32(f32 x, f32 a, f32 b) { return x >= a && x < b; }
GRV_INLINE bool grv_is_in_range_inc_f32(f32 x, f32 a, f32 b) { return x >= a && x <= b; }

u32 grv_dup_u8_u32(u8 a);
u64 grv_dup_u8_u64(u8 a);

GRV_INLINE f32 grv_abs_f32(f32 a) { return a < 0.0f ? -a : a; }
GRV_INLINE f32 grv_abs_s32(s32 a) { return a < 0 ? -a : a; }

GRV_INLINE void grv_swp_s32(s32* a, s32* b) { s32 c = *a; *a = *b; *b = c; }
GRV_INLINE void grv_swp_f32(f32* a, f32* b) { f32 c = *a; *a = *b; *b = c; }

#endif
