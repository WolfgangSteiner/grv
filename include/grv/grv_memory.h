#ifndef GRV_MEMORY_H
#define GRV_MEMORY_H
#include <string.h>
#include "grv_base.h"
#include <stdlib.h>

static inline void grv_memmove(void* dst, void* src, size_t num_elements, size_t element_size) {
    memmove(dst, src, num_elements * element_size);
}

void* grv_alloc(u64 size);
void* grv_alloc_zeros(u64 size);
void* grv_realloc(void* ptr, u64 size);
void grv_free(void* ptr);

#endif