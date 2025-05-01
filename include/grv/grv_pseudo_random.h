#ifndef GRV_PSEUDO_RANDOM_H
#define GRV_PSEUDO_RANDOM_H

#include "grv/grv_base.h"
#include "grv/fx32.h"

extern u32 _grv_pseudo_random_seed_value_u32;
u32 grv_pseudo_random_sequence_u32(u32 index);
void grv_pseudo_random_seed_u32(u32 seed);
u32 grv_pseudo_random_u32(void);
i32 grv_pseudo_random_i32(i32 min_val, i32 max_val);
f32 grv_pseudo_random_f32(void);

#endif
