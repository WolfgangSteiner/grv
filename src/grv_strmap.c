#include "grv_strmap.h"

u64 grv_strmap_hash(grv_str_t* str) {
    u64 hash = 5381;
    size_t len = grv_str_len(str);
    char* cstr = grv_str_cstr(str);

    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + cstr[i];
    }
    return hash;
}

void grv_strmap_init(grv_strmap* map) {
    map->size = 0;
    map->capacity = GRV_STRMAP_DEFAULT_CAPACITY;
    map->arr = calloc(map->capacity, sizeof(grv_strmap_entry));
}

void grv_strmap_free(grv_strmap* map) {
    for (u64 i = 0; i < map->size; i++) {
        grv_strmap_entry* entry = map->arr + i;
        grv_str_free(&entry->key);
        grv_str_free(&entry->value);
    }
    free(map->arr);
}

void grv_strmap_insert(grv_strmap* map, grv_str_t* key, grv_str_t* value) {
    if (map->size == map->capacity) {
        map->capacity *= 2;
        map->arr = realloc(map->arr, map->capacity * sizeof(grv_strmap_entry));
    }

    grv_strmap_entry* entry = map->arr + map->size;
    entry->key = *key;
    entry->value = *value;
    entry->hash = grv_strmap_hash(key);
    map->size++;
}

void grv_strmap_insert_cstr(grv_strmap* map, char* key, char* value) {
    grv_str_t key_str = grv_str_new(key);
    grv_str_t value_str = grv_str_new(value);
    grv_strmap_insert(map, &key_str, &value_str);
}

grv_strmap_entry* grv_strmap_find(grv_strmap* map, grv_str_t* key) {
    u64 hash = grv_strmap_hash(key);
    for (u64 i = 0; i < map->size; i++) {
        grv_strmap_entry* entry = map->arr + i;
        if (entry->hash == hash && grv_str_eq(&entry->key, key)) {
            return entry;
        }
    }
    return NULL;
}

// TODO: Use a refrence instead of copying the cstring into a new grv_str_t
grv_strmap_entry* grv_strmap_find_cstr(grv_strmap* map, char* key) {
    grv_str_t key_str = grv_str_new(key);
    grv_strmap_entry* result = grv_strmap_find(map, &key_str);
    grv_str_free(&key_str);
    return result;
}

grv_str_t* grv_strmap_get(grv_strmap* map, grv_str_t* key) {
    grv_strmap_entry* entry = grv_strmap_find(map, key);
    if (entry) {
        return &entry->value;
    } else {
        return NULL;
    }
}

// TODO: Use a refrence instead of copying the cstring into a new grv_str_t
grv_str_t* grv_strmap_get_cstr(grv_strmap* map, char* key) {
    grv_str_t key_str = grv_str_new(key);
    grv_str_t* result = grv_strmap_get(map, &key_str);
    grv_str_free(&key_str);
    return result;
}

