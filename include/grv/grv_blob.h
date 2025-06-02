#ifndef GRV_BLOB_H
#define GRV_BLOB_H

#include "grv_base.h"

typedef struct {
	u8* data;
	i64 capacity;
	i64 size;
} grv_blob_t;

void grv_blob_push(grv_blob_t* blob, void* data, i64 num_bytes);
void grv_blob_push_u8(grv_blob_t* blob, u8 value);
void grv_blob_push_u32(grv_blob_t* blob, u32 value);
void grv_blob_push_i32(grv_blob_t* blob, i32 value);
void grv_blob_push_f32(grv_blob_t* blob, f32 value);
void grv_blob_push_i64(grv_blob_t* blob, i64 value);
void grv_blob_push_f64(grv_blob_t* blob, f64 value);
void grv_blob_free(grv_blob_t* blob);
#endif
