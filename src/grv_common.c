#include "grv/grv_base.h"
#include <math.h>

s32 round_f32(f32 a) { return (s32)roundf(a); }

s32 min_s32(s32 a, s32 b) { return a < b ? a : b; }
s32 max_s32(s32 a, s32 b) { return a > b ? a : b; }

u32 min_u32(u32 a, u32 b) { return a < b ? a : b; }
u32 max_u32(u32 a, u32 b) { return a > b ? a : b; }

u64 min_u64(u64 a, u64 b) { return a < b ? a : b; }
u64 max_u64(u64 a, u64 b) { return a > b ? a : b; }

size_t min_size_t(size_t a, size_t b) { return a < b ? a : b; }
size_t max_size_t(size_t a, size_t b) { return a > b ? a : b; }

f32 min_f32(f32 a, f32 b) { return a < b ? a : b; }
f32 max_f32(f32 a, f32 b) { return a > b ? a : b; }

u32 dup_u8_u32(u8 a) {
    u32 res = (u32)a;
    res |= res << 8;
    res |= res << 16;
    return res;
}

u64 dup_u8_u64(u8 a) {
    u64 res = (u64)a;
    res |= res << 8;
    res |= res << 16;
    res |= res << 32;
    return res;
}

f32 abs_f32(f32 a) { return a < 0.0f ? -a : a; }
s32 abs_s32(s32 a) { return a < 0 ? -a : a; }

void swp_s32(s32* a, s32* b) { s32 c = *a; *a = *b; *b = c; }
void swp_f32(f32* a, f32* b) { f32 c = *a; *a = *b; *b = c; }
