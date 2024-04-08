#ifndef GRV_ARR_H
#define GRV_ARR_H

#include "grv_base.h"
#include "grv_memory.h"

#define GRV_ARR_INITIAL_CAPACITY 16
#define GRV_ARR_GROWTH_FACTOR 2

typedef struct {
    void* arr;
    size_t size;
    size_t capacity;
} grv_arr_t;

typedef struct {
    void* ptr;
    void* end;
    size_t element_size;
} grv_arr_iter_t;

#define GRV_ARR_TYPE(TYPE) struct {TYPE* arr; size_t size; size_t capacity;} 
#define GRV_ARR_TYPEDEF(TYPE) typedef GRV_ARR_TYPE(TYPE)

GRV_INLINE grv_arr_iter_t _grv_arr_iter_begin(grv_arr_t* arr, size_t element_size) {
    return (grv_arr_iter_t){arr->arr, (u8*)arr->arr + arr->size * element_size, element_size};
}
#define grv_arr_iter_begin(ARR) _grv_arr_iter_begin((grv_arr_t*)ARR, sizeof(*(ARR)->arr))

GRV_INLINE bool grv_arr_iter_is_end(grv_arr_iter_t* iter) {
    return iter->ptr == iter->end;
}

GRV_INLINE void* grv_arr_iter_next(grv_arr_iter_t* iter) {
    if (iter->ptr == iter->end) return NULL;
    void* ptr = iter->ptr;
    iter->ptr = (u8*)iter->ptr + iter->element_size;
    return ptr;
}

#define grv_arr_element_size(ARR) (sizeof(*((ARR)->arr)))

#define grv_arr_init(ARR) do { \
    (ARR)->capacity = GRV_ARR_INITIAL_CAPACITY;\
    (ARR)->arr = grv_alloc((ARR)->capacity * grv_arr_element_size(ARR));\
    (ARR)->size = 0;\
} while (0)

#define grv_arr_check_and_grow(ARR) do { \
    if ((ARR)->capacity == 0) { grv_arr_init(ARR); }\
    else if ((ARR)->size == (ARR)->capacity) { \
        (ARR)->capacity *= GRV_ARR_GROWTH_FACTOR; \
        (ARR)->arr = grv_realloc((ARR)->arr, (ARR)->capacity * grv_arr_element_size(ARR)); \
    } \
} while (0)

#define grv_arr_push(ARR, ELEMENT) do { \
    grv_arr_check_and_grow(ARR); \
    (ARR)->arr[(ARR)->size++] = ELEMENT; \
} while (0)

#define grv_arr_push_front(ARR, ELEMENT) do { \
    grv_arr_check_and_grow(ARR); \
    grv_memmove(&(ARR)->arr[1], (ARR)->arr, (ARR)->size, grv_arr_element_size(ARR)); \
    (ARR)->arr[0] = ELEMENT; \
    (ARR)->size++; \
} while (0)

#define grv_arr_pop(ARR) do { \
    grv_assert((ARR)->size > 0); \
    (ARR)->size--; \
} while (0)

#define grv_arr_pop_front(ARR) do { \
    grv_assert((ARR)->size > 0); \
    grv_memmove((ARR)->arr, (ARR)->arr + 1, (ARR)->size, grv_arr_element_size(ARR)); \
    (ARR)->size--; \
} while (0)

#ifdef __GNUC__
    static inline void* _grv_arr_front(void* arr, size_t size) {
        grv_assert(size > 0);
        return arr;
    }
    static inline void* _grv_arr_back(void* arr, size_t size, size_t element_size) {
        grv_assert(size > 0);
        return (u8*)arr + (size - 1) * element_size;
    }
    static inline void* _grv_arr_at(void* arr, size_t size, size_t index, size_t element_size) {
        grv_assert(index < size);
        return (u8*)arr + index * element_size;
    }
    #define grv_arr_front(ARR) (*((typeof((ARR)->arr)) _grv_arr_front((ARR)->arr, (ARR)->size)))
    #define grv_arr_back(ARR) (*((typeof((ARR)->arr)) _grv_arr_back(((ARR)->arr, (ARR)->size, grv_arr_element_size(ARR)))))
    #define grv_arr_at(ARR, INDEX) (*((typeof((ARR)->arr)) _grv_arr_at((ARR)->arr, (ARR)->size, INDEX, grv_arr_element_size(ARR))))
#else
    #define grv_arr_front(ARR) ((ARR)->arr[0])
    #define grv_arr_back(ARR) ((ARR)->arr[(ARR)->size - 1]) 
    #define grv_arr_at(ARR, INDEX) ((ARR)->arr[INDEX])
#endif

#define grv_arr_free(ARR) do { \
    if ((ARR)->arr) grv_free((ARR)->arr); \
    (ARR)->arr = NULL; \
    (ARR)->size = 0; \
    (ARR)->capacity = 0; \
} while (0)

#endif
