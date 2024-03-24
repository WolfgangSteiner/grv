#include "grv/grv_base.h"
#include <math.h>


u32 grv_dup_u8_u32(u8 a) {
    u32 res = (u32)a;
    res |= res << 8;
    res |= res << 16;
    return res;
}

u64 grv_dup_u8_u64(u8 a) {
    u64 res = (u64)a;
    res |= res << 8;
    res |= res << 16;
    res |= res << 32;
    return res;
}

bool grv_char_is_upper(char c) {
    return c >= 'A' && c <= 'Z';
}

bool grv_char_is_lower(char c) {
    return c >= 'a' && c <= 'z';
}

char grv_char_to_lower(char c) {
    if (grv_char_is_upper(c)) {
        c = c - 'A' + 'a';
    }
    return c;
}

char grv_char_to_upper(char c) {
    if (grv_char_is_lower(c)) {
        c = c - 'a' + 'A';
    }
    return c;
}
