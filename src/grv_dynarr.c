#include "grv/dynarr.h"
#include <stdlib.h>


void grv_dynarr_generic_check_and_grow(grv_dynarr_generic_t* arr, size_t size) {
    if (arr->capacity == 0) {
        arr->capacity = GRV_DYNARR_DEFAULT_CAPACITY;
        arr->data = calloc(arr->capacity, size);
    } else if (arr->size == arr->capacity) {
        arr->capacity *= GRV_DYNARR_GROWTH_FACTOR;
        arr->data = realloc(arr->data, arr->capacity * size);
    }
}

void grv_generic_dynarr_free(grv_dynarr_generic_t* arr) {
    if (arr->data) free(arr->data);
    *arr = (grv_dynarr_generic_t){0};
}   

void grv_dynarr_generic_deep_free(grv_dynarr_generic_t* arr, void (*free_func)(void*)) {
    for (int i = 0; i < arr->size; i++) {
        void* ptr = (void*)arr->data[i];
        free_func(ptr);
    }
    grv_dynarr_generic_free(arr);
}


