#ifndef GRV_COMMON_H
#define GRV_COMMON_H

#include "grv_base.h"

s32 round_f32(f32 a);

GRV_INLINE s32 min_s32(s32 a, s32 b) { return a < b ? a : b; }
GRV_INLINE s32 max_s32(s32 a, s32 b) { return a > b ? a : b; }

GRV_INLINE u32 min_u32(u32 a, u32 b) { return a < b ? a : b; }
GRV_INLINE u32 max_u32(u32 a, u32 b) { return a > b ? a : b; }

GRV_INLINE s64 min_s64(s64 a, s64 b) { return a < b ? a : b; }
GRV_INLINE s64 max_s64(s64 a, s64 b) { return a > b ? a : b; }

GRV_INLINE size_t min_size_t(size_t a, size_t b) { return a < b ? a : b; }
GRV_INLINE size_t max_size_t(size_t a, size_t b) { return a > b ? a : b; }

GRV_INLINE f32 min_f32(f32 a, f32 b) { return a < b ? a : b; }
GRV_INLINE f32 max_f32(f32 a, f32 b) { return a > b ? a : b; }

u32 dup_u8_u32(u8 a);
u64 dup_u8_u64(u8 a);

s32 abs_s32(s32 a);
f32 abs_f32(f32 a);

void swp_s32(s32* a, s32* b);
void swp_f32(f32* a, f32* b);

#endif
