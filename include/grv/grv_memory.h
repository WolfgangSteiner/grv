#ifndef GRV_MEMORY_H
#define GRV_MEMORY_H
#include <string.h>
#include "grv_base.h"
#include <stdlib.h>

#define GRV_KILOBYTES 1024ull
#define GRV_MEGABYTES (GRV_KILOBYTES * GRV_KILOBYTES)
#define GRV_GIGABYTES (GRV_KILOBYTES * GRV_MEGABYTES)

typedef struct grv_allocator_s {
    void*(*alloc)(struct grv_allocator_s*, size_t);
    void*(*alloc_zero)(struct grv_allocator_s*, size_t);
    void(*free)(struct grv_allocator_s*, void*);
    void*(*realloc)(struct grv_allocator_s*, void*, size_t);
    void(*deinit)(struct grv_allocator_s*);
    void* impl;
} grv_allocator_t;

GRV_INLINE void* grv_allocator_alloc(grv_allocator_t* allocator, size_t size) {
    return allocator->alloc(allocator, size);
}
GRV_INLINE void* grv_allocator_alloc_zero(grv_allocator_t* allocator, size_t size) {
    return allocator->alloc_zero(allocator, size);
}
GRV_INLINE void grv_allocator_free(grv_allocator_t* allocator, void* ptr) {
    allocator->free(allocator, ptr);
}
GRV_INLINE void* grv_allocator_realloc(grv_allocator_t* allocator, void* ptr, size_t size) {
    return allocator->realloc(allocator, ptr, size);
}
GRV_INLINE void grv_allocator_deinit(grv_allocator_t* allocator) {
    allocator->deinit(allocator);
}

#define GRV_ALLOC_OBJECT_ZERO(TYPE) grv_alloc_zeros(sizeof(TYPE))
#define GRV_ALLOC_OBJECT(TYPE) grv_alloc(sizeof(TYPE))

grv_allocator_t* grv_default_allocator(void);


static inline void grv_memmove(void* dst, void* src, size_t num_elements, size_t element_size) {
    memmove(dst, src, num_elements * element_size);
}

GRV_INLINE void grv_memcpy(void* dst, void* src, size_t size) {
    memcpy(dst, src, size);
}

void* grv_alloc(u64 size);
void* grv_alloc_zeros(u64 size);
void* grv_realloc(void* ptr, u64 size);
void grv_free(void* ptr);

#endif
