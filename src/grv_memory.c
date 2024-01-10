#include "grv/grv_memory.h"
#include "grv/grv_common.h"


static size_t _grv_calc_alloc_size(size_t size) {
  size_t mask = sizeof(void*) - 1;
  size_t result = (size + mask) & ~mask;
  #ifdef GRV_DEBUG_MEMORY
    result += sizeof(void*);
  #endif
  return result;
}

#ifdef GRV_DEBUG_MEMORY
  static size_t grv_impl_get_alloc_size(void* ptr) {
    size_t* alloc_ptr = ptr;
    alloc_ptr--;
    return *alloc_ptr;
  }

  static size_t* grv_impl_get_alloc_ptr(void* ptr) {
    size_t* alloc_ptr = ptr;
    alloc_ptr--;
    return alloc_ptr;
  }
#endif


void* grv_alloc(u64 size) {
  size_t alloc_size = _grv_calc_alloc_size(size);
  size_t* alloc_ptr = malloc(alloc_size);
  void* result = alloc_ptr;
  if (result == NULL) return result;
  #ifdef GRV_ZERO_MEMORY
    memset(alloc_ptr, 0, alloc_size);
  #elif defined(GRV_DEBUG_MEMORY)
    memset(alloc_ptr, 0xef, alloc_size);
    *alloc_ptr = alloc_size;
    result = alloc_ptr + 1;
  #endif
  return result;
}

void* grv_alloc_zeros(u64 size) {
  void* ptr = grv_alloc(size);
  #ifndef GRV_ZERO_MEMORY
    if (ptr == NULL) return ptr;
    memset(ptr, 0, size);
  #endif
  return ptr;
}

void* grv_realloc(void* ptr, u64 size) {
  assert(ptr != NULL);
  assert(size != 0L);
#ifdef GRV_DEBUG_MEMORY
  // always realloc to a new pointer
  void* new_ptr = grv_alloc(size);
  if (new_ptr == NULL) return NULL;
  size_t old_alloc_size = grv_impl_get_alloc_size(ptr);
  size_t new_alloc_size = grv_impl_get_alloc_size(new_ptr);
  size_t cpy_size = min_u64(old_alloc_size, new_alloc_size) - sizeof(size_t);
  memcpy(new_ptr, ptr, cpy_size);
  grv_free(ptr);
  return new_ptr;
#else
  return realloc(ptr, size);
#endif
}

void grv_free(void* ptr) {
  assert(ptr != NULL);
  #ifdef GRV_DEBUG_MEMORY
    size_t alloc_size = grv_impl_get_alloc_size(ptr);
    size_t* alloc_ptr = grv_impl_get_alloc_ptr(ptr);
    memset(alloc_ptr, 0xcd, alloc_size); 
    free(alloc_ptr);
  #else
    free(ptr); 
  #endif
}

void grv_free_prepare(void* ptr) {
  (void)ptr;
  #ifdef GRV_DEBUG_MEMORY
    size_t alloc_size = grv_impl_get_alloc_size(ptr);
    size_t* alloc_ptr = grv_impl_get_alloc_ptr(ptr);
    memset(alloc_ptr, 0xcd, alloc_size); 
  #endif
}