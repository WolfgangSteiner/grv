#include "grv/grv_strmap.h"
#include "grv/grv_arr.h"

u64 grv_strmap_hash(grv_str_t str) {
    u64 hash = 5381;
    size_t len = grv_str_len(str);
    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + str.data[i];
    }
    return hash;
}

void grv_strmap_init(grv_strmap_t* map) {
    map->size = 0;
    map->capacity = GRV_STRMAP_DEFAULT_CAPACITY;
    map->arr = calloc(map->capacity, sizeof(grv_strmap_entry_t));
}

void grv_strmap_free(grv_strmap_t* map) {
    for (u64 i = 0; i < map->size; i++) {
        grv_strmap_entry_t* entry = map->arr + i;
        grv_str_free(&entry->key);
        grv_str_free(&entry->value);
    }
    free(map->arr);
}

void grv_strmap_insert_str(grv_strmap_t* map, grv_str_t key, grv_str_t value) {
    grv_strmap_entry_t entry = {.key=key, .value=value, .hash=grv_strmap_hash(key)};
    grv_arr_push(map, entry);
}

grv_strmap_entry_t* grv_strmap_find_str(grv_strmap_t* map, grv_str_t key) {
    u64 hash = grv_strmap_hash(key);
    grv_arr_iter_t iter = grv_arr_iter_begin(map);
    grv_strmap_entry_t* entry = NULL;
    while((entry = grv_arr_iter_next(&iter)) != NULL) {
        if (entry->hash == hash && grv_str_eq(entry->key, key)) return entry;
    }    
    return NULL;
}

grv_str_t grv_strmap_get_str(grv_strmap_t* map, grv_str_t key, grv_str_t default_str) {
    grv_strmap_entry_t* entry = grv_strmap_find(map, key);
    return entry ? entry->value : default_str;
}

