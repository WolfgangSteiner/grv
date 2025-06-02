#include "grv/grv_hash.h"
#include <string.h>

u32 grv_hash_fnv(void* data, i64 num_bytes) {
	if (!data) return 0;
	u8* src = data;
	u32 hash = 0x811C9DC5;
	for (i64 i = 0; i < num_bytes; i++) {
		hash ^= *src++;
		hash *= 0x01000193;
	}
	return hash;
}

u32 grv_hash_fnv_cstr(char* str) {
	i32 len = strlen(str);
	return	grv_hash_fnv(str, len);
}
