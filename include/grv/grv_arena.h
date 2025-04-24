#ifndef GRV_ARENA_H
#define GRV_ARENA_H
#include "grv/grv_base.h"
#include "grv/grv_common.h"
#include "libc/memset.h"

typedef struct {
	u8* data;
	size_t capacity;
	size_t size;
	size_t max_size;
	size_t* frame_stack;
	size_t frame_count;
	size_t frame_stack_capacity;
} grv_arena_t;

void grv_arena_init(grv_arena_t* arena, size_t capacity);

GRV_INLINE void* grv_arena_alloc(grv_arena_t* arena, size_t size) {
    size = (size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);
	assert(arena->size + size <= arena->capacity);
	void* res = arena->data + arena->size;
	arena->size += size;
	arena->max_size = grv_max_u64(arena->max_size, arena->size);
	return res;
}

GRV_INLINE void* grv_arena_alloc_zero(grv_arena_t* arena, size_t size) {
	void* ptr = grv_arena_alloc(arena, size);
	memset(ptr, 0, size);
	return ptr;
}

GRV_INLINE void grv_arena_free(grv_arena_t* arena, size_t size) {
    size = (size + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1);
	arena->size -= size;
}

GRV_INLINE void grv_arena_reset(grv_arena_t* arena) {
	arena->size = 0;
	arena->frame_count = 0;
}

GRV_INLINE void grv_arena_push_frame(grv_arena_t* arena) {
	arena->frame_stack[arena->frame_count++] = arena->size;
}

GRV_INLINE void grv_arena_pop_frame(grv_arena_t* arena) {
	assert(arena->frame_count);
	arena->size = arena->frame_stack[--arena->frame_count];
}

#endif
