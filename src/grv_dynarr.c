#include "grv/dynarr.h"
#include <stdlib.h>
void grv_dynarr_init(grv_dynarr* dynarr) {
    dynarr->size = 0;
    dynarr->capacity = GRV_DYNARR_DEFAULT_CAPACITY;
    dynarr->data = grv_malloc(dynarr->capacity * sizeof(void*));
}
void grv_dynarr_free(grv_dynarr* dynarr) {
    grv_free(dynarr->data);
}
void grv_dynarr_deep_free(grv_dynarr* dynarr, void (*free_func)(void*)) {
    for (int i = 0; i < dynarr->size; i++) {
        free_func(dynarr->data[i]);
    }
    grv_free(dynarr->data);
}
void grv_dynarr_push(grv_dynarr* dynarr, void* data) {
    if (dynarr->size == dynarr->capacity) {
        dynarr->capacity *= GRV_DYNARR_GROWTH_FACTOR;
        dynarr->data = grv_realloc(dynarr->data, dynarr->capacity * sizeof(void*));
    }
    dynarr->data[dynarr->size++] = data;
}
void* grv_dynarr_pop(grv_dynarr* dynarr) {
    if (dynarr->size == 0) {
        return NULL;
    }
    return dynarr->data[--dynarr->size];
}
void grv_dynarr_clear(grv_dynarr* dynarr) {
    dynarr->size = 0;
}
void grv_dynarr_reset(grv_dynarr* dynarr) {
    dynarr->size = 0;
    dynarr->capacity = GRV_DYNARR_DEFAULT_CAPACITY;
    dynarr->data = grv_realloc(dynarr->data, dynarr->capacity * sizeof(void*));
}