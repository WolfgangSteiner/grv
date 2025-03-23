#include "grv/grv_pseudo_random.h"

u32 _grv_pseudo_random_seed_value_u32 = 42;

u32 grv_pseudo_random_sequence_u32(u32 index) {
    index ^= index << 13;
    index ^= index >> 17;
    index ^= index << 5;
    return index;
}

void grv_pseudo_random_seed_u32(u32 seed) { _grv_pseudo_random_seed_value_u32 = seed; }

u32 grv_pseudo_random_u32(void) {
    _grv_pseudo_random_seed_value_u32 = 
        grv_pseudo_random_sequence_u32(_grv_pseudo_random_seed_value_u32);
    return _grv_pseudo_random_seed_value_u32;
}

i32 grv_pseudo_random_i32(i32 min_val, i32 max_val) {
    u32 val_range = (u32)(max_val - min_val + 1);
    u32 rand_val = grv_pseudo_random_u32() % val_range;
    return min_val + (i32)rand_val;
}
