#include "grv/grv_arr.h"

void _grv_arr_init(grv_arr_t* arr, size_t element_size, size_t initial_capacity) {
    arr->arr = grv_alloc(initial_capacity * element_size);
    arr->capacity = initial_capacity;
    arr->size = 0;
}   

void _grv_arr_check_and_grow(grv_arr_t* arr, size_t element_size, size_t initial_capacity, size_t growth_factor) {
    if (arr->capacity == 0) {
        arr->capacity = initial_capacity;
        arr->arr = grv_alloc(arr->capacity * element_size);
    } else if (arr->size == arr->capacity) {
        arr->capacity *= growth_factor;
        arr->arr = grv_realloc(arr->arr, arr->capacity * element_size);
    }
}

void _grv_arr_push_front(grv_arr_t* arr, size_t element_size, void* element) {
    _grv_arr_check_and_grow(arr, element_size, GRV_ARR_DEFAULT_CAPACITY, GRV_ARR_GROWTH_FACTOR);
    memmove((char*)arr->arr + element_size, arr->arr, arr->size * element_size);
    memcpy(arr->arr, element, element_size);
    arr->size++;
}

void _grv_arr_push(grv_arr_t* arr, size_t element_size, void* element) {
    _grv_arr_check_and_grow(arr, element_size, GRV_ARR_DEFAULT_CAPACITY, GRV_ARR_GROWTH_FACTOR);
    memcpy((char*)arr->arr + arr->size * element_size, element, element_size);
    arr->size++;
}

void _grv_arr_pop(grv_arr_t* arr) {
    grv_assert(arr->size > 0);
    arr->size--;
}

void _grv_arr_pop_front(grv_arr_t* arr, size_t element_size) {
    grv_assert(arr->size > 0);
    grv_memmove(arr->arr, (char*)arr->arr + element_size, arr->size, element_size);
    arr->size--;
}

void _grv_arr_free(grv_arr_t* arr) {
    if (arr->arr) grv_free(arr->arr);
    *arr = (grv_arr_t){0};
}