#ifndef GRV_DYNARR_H
#define GRV_DYNARR_H
#include "grv_base.h"
#define GRV_DYNARR_DEFAULT_CAPACITY 16
#define GRV_DYNARR_GROWTH_FACTOR 2
typedef struct {
    size_t size;
    size_t capacity;
    void** data;
} grv_dynarr;

void grv_dynarr_init(grv_dynarr* dynarr);
void grv_dynarr_free(grv_dynarr* dynarr);
void grv_dynarr_deep_free(grv_dynarr* dynarr, void (*free_func)(void*));
void grv_dynarr_push(grv_dynarr* dynarr, void* data);
void* grv_dynarr_pop(grv_dynarr* dynarr);
void grv_dynarr_clear(grv_dynarr* dynarr);
void grv_dynarr_reset(grv_dynarr* dynarr);
#endif