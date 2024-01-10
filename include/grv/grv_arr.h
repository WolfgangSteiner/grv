#ifndef GRV_ARR_H
#define GRV_ARR_H

#include "grv_base.h"
#include "grv_memory.h"

#define GRV_ARR_DEFAULT_CAPACITY 16
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

GRV_INLINE grv_arr_iter_t _grv_arr_iter_begin(grv_arr_t* arr, size_t element_size) {
    return (grv_arr_iter_t){arr->arr, arr->arr + arr->size * element_size, element_size};
}
#define grv_arr_iter_begin(ARR) _grv_arr_iter_begin((grv_arr_t*)ARR, sizeof(*(ARR)->arr))

GRV_INLINE bool grv_arr_iter_is_end(grv_arr_iter_t* iter) {
    return iter->ptr == iter->end;
}

GRV_INLINE void* grv_arr_iter_next(grv_arr_iter_t* iter) {
    if (iter->ptr == iter->end) return NULL;
    void* ptr = iter->ptr;
    iter->ptr += iter->element_size;
    return ptr;
}

void _grv_arr_init(grv_arr_t* arr, size_t element_size, size_t initial_capacity);
#define grv_arr_init(ARR) _grv_arr_init((grv_arr_t*)ARR, sizeof(*(ARR)->arr), GRV_ARR_DEFAULT_CAPACITY)

void _grv_arr_check_and_grow(grv_arr_t* arr, size_t element_size, size_t initial_capacity, size_t growth_factor);
#define grv_arr_check_and_grow(ARR) \
    _grv_arr_check_and_grow((grv_arr_t*)ARR, sizeof(*(ARR)->arr), GRV_ARR_DEFAULT_CAPACITY, GRV_ARR_GROWTH_FACTOR)

void _grv_arr_push(grv_arr_t* arr, size_t element_size, void* element);
#define grv_arr_push(ARR, ELEMENT) _grv_arr_push((grv_arr_t*)ARR, sizeof(*ARR->arr), &ELEMENT);

void _grv_arr_push_front(grv_arr_t* arr, size_t element_size, void* element);
#define grv_arr_push_front(ARR, ELEMENT) _grv_arr_push_front((grv_arr_t*)ARR, sizeof(*ARR->arr), &ELEMENT);

void _grv_arr_pop(grv_arr_t* arr);
#define grv_arr_pop(ARR) _grv_arr_pop((grv_arr_t*)ARR);

void _grv_arr_pop_front(grv_arr_t* arr, size_t element_size);
#define grv_arr_pop_front(ARR) _grv_arr_pop_front((grv_arr_t*)ARR, sizeof(*ARR->arr));

#ifdef __GNUC__
    static inline void* _grv_arr_front(grv_arr_t* arr) {
        grv_assert(arr->size > 0);
        return arr->arr;
    }
    static inline void* _grv_arr_back(grv_arr_t* arr, size_t element_size) {
        grv_assert(arr->size > 0);
        return arr->arr + (arr->size - 1) * element_size;
    }
    static inline void* _grv_arr_at(grv_arr_t* arr, size_t index, size_t element_size) {
        grv_assert(index < arr->size);
        return arr->arr + index * element_size;
    }
    #define grv_arr_front(ARR) (*((typeof(ARR->arr)) _grv_arr_front((grv_arr_t*)ARR)))
    #define grv_arr_back(ARR) (*((typeof(ARR->arr)) _grv_arr_back((grv_arr_t*)ARR, sizeof(*ARR->arr))))
    #define grv_arr_at(ARR, INDEX) (*((typeof(ARR->arr)) _grv_arr_at((grv_arr_t*)ARR, INDEX, sizeof(*ARR->arr))))
#else
    #define grv_arr_front(ARR) (ARR->arr[0])
    #define grv_arr_back(ARR) (ARR->arr[ARR->size - 1]) 
    #define grv_arr_at(ARR, INDEX) (ARR->arr[INDEX])
#endif

void _grv_arr_free(grv_arr_t* arr);
#define grv_arr_free(ARR) _grv_arr_free((grv_arr_t*)ARR);

#endif