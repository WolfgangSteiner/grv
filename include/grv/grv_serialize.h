#ifndef GRV_SERIALIZE_H
#define GRV_SERIALIZE_H

#include "grv_base.h"
#include "grv_blob.h"
#include "grv_dynarr.h"

#define GRV_SERIALIZE_MAX_LENGTH_ID 32
#define GRV_SERIALIZER_VERSION 0
typedef enum {
	GRV_SERIALIZER_TOKEN_U8,
	GRV_SERIALIZER_TOKEN_I32,
	GRV_SERIALIZER_TOKEN_F32,
	GRV_SERIALIZER_TOKEN_I64,
	GRV_SERIALIZER_TOKEN_F64,
	GRV_SERIALIZER_TOKEN_INT,
	GRV_SERIALIZER_TOKEN_FLOAT,
	GRV_SERIALIZER_TOKEN_BOOL,
	GRV_SERIALIZER_TOKEN_STRING,
	GRV_SERIALIZER_TOKEN_VERSION,
	GRV_SERIALIZER_TOKEN_ARRAY_BEGIN,
	GRV_SERIALIZER_TOKEN_ARRAY_END,
	GRV_SERIALIZER_TOKEN_STRUCT_BEGIN,
	GRV_SERIALIZER_TOKEN_STRUCT_FIELD,
	GRV_SERIALIZER_TOKEN_STRUCT_END,
	GRV_SERIALIZER_TOKEN_KEY,
	GRV_SERIALIZER_TOKEN_BLOB_BEGIN,
	GRV_SERIALIZER_TOKEN_BLOB_END,
	GRV_SERIALIZER_TOKEN_SYMBOLS_BEGIN,
	GRV_SERIALIZER_TOKEN_SYMBOLS_END,
	GRV_SERIALIZER_TOKEN_DATA_BEGIN,
	GRV_SERIALIZER_TOKEN_DATA_END,
	GRV_SERIALIZER_TOKEN_CHUNK_BEGIN,
	GRV_SERIALIZER_TOKEN_CHUNK_END,
	GRV_SERIALIZER_TOKEN_VERSION,
	GRV_SERIALIZER_TOKEN_COUNT,
} grv_serializer_token_t;

typedef struct {
	u32 hash;
	char* symbol;
} grv_serializer_symbol_t;

typedef struct {
	grv_dynarr_u32_t symbol_hashes;
	grv_dynarr_u32_t index_stack;
	grv_blob_t symbols;
	grv_blob_t data;
	i32 version;
	i64 _read_idx;
} grv_serializer_t;


void grv_serializer_free(grv_serializer_t* s);
void grv_serialize_u8(grv_serializer_t* s, u8 value);
void grv_serialize_i32(grv_serializer_t* s, i32 value);
void grv_serialize_f32(grv_serializer_t* s, f32 value);
void grv_serialize_i64(grv_serializer_t* s, i64 value);
void grv_serialize_f64(grv_serializer_t* s, f64 value);
void grv_serialize_int(grv_serializer_t* s, i32 value);
void grv_serialize_float(grv_serializer_t* s, f32 value);
void grv_serialize_bool(grv_serializer_t* s, bool value);
void grv_serialize_array_begin(grv_serializer_t* s, i64 size);
void grv_serialize_array_end(grv_serializer_t* s);
void grv_serialize_struct_begin(grv_serializer_t* s, u32 version);
void grv_serialize_struct_end(grv_serializer_t* s);
void grv_serialize_struct_field(grv_serializer_t* s, char* name);
void grv_serialize_struct_field_bool(grv_serializer_t* s, char* name, bool value);
void grv_serialize_struct_field_int(grv_serializer_t* s, char* name, i32 value);
void grv_serialize_struct_field_float(grv_serializer_t* s, char* name, f32 value);

bool grv_serializer_write(grv_serializer_t* s, char* filename);

bool grv_deserialize_struct_begin(grv_serializer_t* s, u32* version);
bool grv_deserialize_struct_field_next(grv_serializer_t* s, char** id);
bool grv_deserialize_struct_end(grv_serializer_t* s);

#endif
