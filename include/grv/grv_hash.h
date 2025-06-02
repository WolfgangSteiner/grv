#ifndef GRV_HASH_H
#define GRV_HASH_H

#include "grv_base.h"
u32 grv_hash_fnv(void* data, i64 num_bytes);
u32 grv_hash_fnv_cstr(char* str);

#endif
