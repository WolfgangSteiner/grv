#include "grv/grv_blob.h"
#include "grv/grv_memory.h"
#include "grv/grv_common.h"

void grv_blob_push(grv_blob_t* blob, void* data, i64 num_bytes) {
	while (blob->size + num_bytes > blob->capacity) {
		blob->capacity = grv_max_i64(2 * blob->capacity, 1024);
		blob->data = grv_realloc(blob->data, blob->capacity);
	}
	u8* dst = blob->data + blob->size;
	memcpy(dst, data, num_bytes);
	blob->size += num_bytes;
}

void grv_blob_push_u8(grv_blob_t* blob, u8 value) {
	grv_blob_push(blob, &value, sizeof(u8));
}

void grv_blob_push_u32(grv_blob_t* blob, u32 value) {
	grv_blob_push(blob, &value, sizeof(u32));
}

void grv_blob_push_i32(grv_blob_t* blob, i32 value) {
	grv_blob_push(blob, &value, sizeof(i32));
}

void grv_blob_push_f32(grv_blob_t* blob, f32 value) {
	grv_blob_push(blob, &value, sizeof(f32));
}

void grv_blob_push_i64(grv_blob_t* blob, i64 value) {
	grv_blob_push(blob, &value, sizeof(i64));
}

void grv_blob_push_f64(grv_blob_t* blob, f64 value) {
	grv_blob_push(blob, &value, sizeof(f64));
}

void grv_blob_free(grv_blob_t* blob) {
	if (blob->data) grv_free(blob->data);
	*blob = (grv_blob_t) {0};
}
