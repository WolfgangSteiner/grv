#ifndef GRV_COMMON_H
#define GRV_COMMON_H

#include "grv_base.h"

s32 round_f32(f32 a);

s32 min_s32(s32 a, s32 b);
s32 max_s32(s32 a, s32 b);

u32 min_u32(u32 a, u32 b);
u32 max_u32(u32 a, u32 b);

u64 min_u64(u64 a, u64 b);
u64 max_u64(u64 a, u64 b);

size_t min_size_t(size_t, size_t);
size_t max_size_t(size_t, size_t);

f32 min_f32(f32 a, f32 b);
f32 max_f32(f32 a, f32 b);

u32 dup_u8_u32(u8 a);
u64 dup_u8_u64(u8 a);

s32 abs_s32(s32 a);
f32 abs_f32(f32 a);

void swp_s32(s32* a, s32* b);
void swp_f32(f32* a, f32* b);

#endif
