#include "grv/grv_arena.h"
#include "grv/grv_memory.h"

void grv_arena_init(grv_arena_t* arena, size_t capacity) {
	*arena = (grv_arena_t) {0};
	arena->data = grv_alloc(capacity);
	arena->capacity = capacity;
	arena->frame_stack_capacity = 1024;
	arena->frame_stack = grv_alloc(arena->frame_stack_capacity * sizeof(size_t));
}
