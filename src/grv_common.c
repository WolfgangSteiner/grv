#include "grv/grv_base.h"
#include <math.h>

s32 round_f32(f32 a) { return (s32)roundf(a); }


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
