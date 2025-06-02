#include "grv/grv_serialize.h"
#include "grv/grv_common.h"
#include "grv/grv_memory.h"
#include "grv/grv_hash.h"
#include <string.h>
#include <stdio.h>

#define GRV_SERIALIZE_DEFAULT_DATA_CAPACITY 4096

void grv_serializer_free(grv_serializer_t* s) {
	grv_dynarr_u32_free(&s->symbol_hashes);
	grv_blob_free(&s->symbols);
	grv_blob_free(&s->data);
}

void grv_serializer_push_u8(grv_serializer_t* s, u8 value) {
	grv_blob_push_u8(&s->data, value);
}

void grv_serializer_push_token(grv_serializer_t* s, grv_serializer_token_t token) {
	grv_serializer_push_u8(s, token);
}

void grv_serialize_u8(grv_serializer_t* s, u8 value) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_U8);
	grv_blob_push_u8(&s->data, value);
}

void grv_serialize_i32(grv_serializer_t* s, i32 value) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_I32);
	grv_blob_push_i32(&s->data, value);
}

void grv_serialize_f32(grv_serializer_t* s, f32 value) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_F32);
	grv_blob_push_f32(&s->data, value);
}

void grv_serialize_i64(grv_serializer_t* s, i64 value) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_I64);
	grv_blob_push_i64(&s->data, value);
}

void grv_serialize_f64(grv_serializer_t* s, f64 value) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_F64);
	grv_blob_push_f64(&s->data, value);
}

void grv_serialize_int(grv_serializer_t* s, i32 value) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_INT);
	grv_blob_push_i32(&s->data, value);
}

void grv_serialize_float(grv_serializer_t* s, f32 value) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_FLOAT);
	grv_blob_push_f32(&s->data, value);
}

void grv_serialize_bool(grv_serializer_t* s, bool value) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_BOOL);
	grv_blob_push_u8(&s->data, (u8)value);
}

void grv_serializer_push_size(grv_serializer_t* s, i64 size) {
	grv_blob_push_i64(&s->data, size);
}

void grv_serializer_push_version(grv_serializer_t* s, u32 version) {
	if (version) {
		grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_VERSION);
		grv_blob_push_u32(&s->data, version);
	}
}

void grv_serialize_array_begin(grv_serializer_t* s, i64 size) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_ARRAY_BEGIN);
	grv_serializer_push_size(s, size);
}

void grv_serialize_array_end(grv_serializer_t* s) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_ARRAY_END);
}




void grv_serialize_struct_begin(grv_serializer_t* s, u32 version) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_STRUCT_BEGIN);
	grv_dynarr_u32_push(&s->index_stack, s->data.size);
	grv_blob_push_u32(&s->data, 0);
	grv_serializer_push_version(s, version);
}

void grv_serialize_struct_end(grv_serializer_t* s) {
	u32 start_idx = grv_dynarr_u32_pop(&s->index_stack);
	s->data.data[start_idx] = s->data.size - start_idx - sizeof(u32);
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_STRUCT_END);
}

bool grv_serializer_has_symbol(grv_serializer_t* s, u32 hash) {
	for (i32 i = 0; i < s->symbol_hashes.size; i++) {
		if (s->symbol_hashes.arr[i] == hash) return true;
	}
	return false;
}

void grv_serializer_push_symbol(grv_serializer_t* s, char* symbol, u32 hash) {
	grv_dynarr_u32_push(&s->symbol_hashes, hash);
	grv_blob_push_u32(&s->symbols, hash);
	i64 len = strlen(symbol);
	grv_assert(len < 255);
	grv_blob_push_u8(&s->symbols, len + 1);
	grv_blob_push(&s->symbols,symbol, len + 1);
}

u32 grv_serializer_add_symbol(grv_serializer_t* s, char* name) {
	u32 hash = grv_hash_fnv_cstr(name);
	if (!grv_serializer_has_symbol(s, hash)) {
		grv_serializer_push_symbol(s, name, hash);
	}
	return hash;
}

char* grv_serializer_get_symbol(grv_serializer_t* s, u32 hash) {
	i64 read_idx = 0;
	while (read_idx < s->symbols.size - sizeof(hash)) {
		u32 i_hash;
		memcpy(&i_hash, s->symbols.data + read_idx, sizeof(hash));
		assert(read_idx < s->symbols.size - 1);
		read_idx += sizeof(hash);
		u8 len = s->symbols.data[read_idx++];
		if (i_hash == hash) {
			return (char*)s->symbols.data + read_idx;
		}
	}
	return NULL;
}

void grv_serialize_struct_field(grv_serializer_t* s, char* name) {
	u32 hash = grv_serializer_add_symbol(s, name);
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_STRUCT_FIELD);
	grv_blob_push_u32(&s->data, hash);
}

void grv_serialize_struct_field_bool(grv_serializer_t* s, char* name, bool value) {
	grv_serialize_struct_field(s, name);
	grv_serialize_bool(s, value);
}

void grv_serialize_struct_field_int(grv_serializer_t* s, char* name, i32 value) {
	grv_serialize_struct_field(s, name);
	grv_serialize_int(s, value);
}

void grv_serialize_struct_field_float(grv_serializer_t* s, char* name, f32 value) {
	grv_serialize_struct_field(s, name);
	grv_serialize_float(s, value);
}

void grv_serialize_blob(grv_serializer_t* s, u8* data, u32 num_bytes, u32 version) {
	grv_serializer_push_token(s, GRV_SERIALIZER_TOKEN_BLOB_BEGIN);
	grv_blob_push_u32(&s->data, num_bytes);
	if (version) {
		
	}
}

void _grv_serializer_write_token(FILE* fp, u32 token) {
	fputc((u8)token, fp);
}

bool grv_serializer_write(grv_serializer_t* s, char* filename) {
	FILE* fp = fopen(filename, "wb");
	fwrite("GRV_SER", 1, 8, fp);
	fputc(GRV_SERIALIZER_VERSION, fp);
	if (!fp) return false;
	fputc(GRV_SERIALIZER_TOKEN_SYMBOLS_BEGIN,fp);
	fwrite(s->symbols.data, 1, s->symbols.size, fp);
	fputc(GRV_SERIALIZER_TOKEN_SYMBOLS_END,fp);

	fputc(GRV_SERIALIZER_TOKEN_DATA_BEGIN,fp);
	fwrite(s->data.data, 1, s->data.size, fp);
	fputc(GRV_SERIALIZER_TOKEN_DATA_END,fp);

	fclose(fp);
	return true;
}

bool grv_deserialize_read_token(grv_serializer_t* s, u8* token) {
	if (s->_read_idx >= s->data.capacity) return false;
	*token = s->data.data[s->_read_idx++];
	if (*token > (u8)GRV_SERIALIZER_TOKEN_COUNT) return false;
	return true;
}

bool grv_deserialize_assert_token(grv_serializer_t* s, u8 expected) {
	u8 token;
	if (!grv_deserialize_read_token(s, &token)) return false;
	return token == expected;
}

bool grv_deserialize_match_token(grv_serializer_t* s, u8 match) {
	if (s->_read_idx >= s->data.capacity) return false;
	u8 token = s->data.data[s->_read_idx];
	if (token == match) {
		s->_read_idx++;
		return true;
	} else {
		return true;
	}
}

bool grv_deserialize_assert_available_bytes(grv_serializer_t* s, i64 num_bytes) {
	return s->data.capacity - s->_read_idx >= num_bytes;
}

bool grv_deserialize_read_bytes(grv_serializer_t* s, void* dst, i64 num_bytes) {
	if (!grv_deserialize_assert_available_bytes(s, num_bytes)) return false;
	memcpy(dst, s->data.data + s->_read_idx, num_bytes);
	return true;
}

bool grv_deserialize_read_raw_u32(grv_serializer_t* s, u32* value) {
	return grv_deserialize_read_bytes(s, value, sizeof(u32));
}

bool grv_deserialize_struct_begin(grv_serializer_t* s, u32* version) {
	u32 version_val = 0;
	u32 struct_size;
	bool success =
		grv_deserialize_assert_token(s, GRV_SERIALIZER_TOKEN_STRUCT_BEGIN)
		&& grv_deserialize_read_bytes(s, &struct_size, sizeof(u32));
	if (success && grv_deserialize_match_token(s, GRV_SERIALIZER_TOKEN_STRUCT_VERSION)) {
		success = grv_deserialize_read_raw_u32(s, &version_val);
	}
	if (version) *version = version_val;
	return success;
}

bool grv_deserialize_blob_read(grv_serializer_t* s, u8* data, u32* num_bytes, u32* version) {
	u32 version_val = 0;
	u32 blob_size;
	bool success =
		grv_deserialize_assert_token(s, GRV_SERIALIZER_TOKEN_BLOB_BEGIN)
		&& grv_deserialize_read_bytes(s, num_bytes, sizeof(u32));
	if (success && grv_deserialize_match_token(s, GRV_SERIALIZER_TOKEN_VERSION)) {
		success = grv_deserialize_read_raw_u32(s, &version_val);
	}
	if (version) *version = version_val;
	data = success ? s->data.data + s->_read_idx: NULL;
	return success;
}

bool grv_deserialize_struct_field_next(grv_serializer_t* s, char** id) {
	*id = NULL;
	bool success = true;
	if (grv_deserialize_match_token(s, GRV_SERIALIZER_TOKEN_STRUCT_FIELD)) {
		u32 hash;
		success = grv_deserialize_read_raw_u32(s, &hash);
		if (success == false) return false;
		char* symbol_name = grv_serializer_get_symbol(s, hash);
		if (symbol_name == NULL) return false;
		*id = symbol_name;
	}

	return success;
}

bool grv_deserialize_struct_end(grv_serializer_t* s) {
	return grv_deserialize_assert_token(s, GRV_SERIALIZER_TOKEN_STRUCT_END);
}

bool grv_deserialize_array_begin(grv_serializer_t* s, i64* size) {
	bool success = grv_deserialize_assert_token(s, GRV_SERIALIZER_TOKEN_ARRAY_BEGIN);
	success = grv_deserialize_read_bytes(s, size, sizeof(i64));
	return success;
}

#if 0
void grv_serialize_size(grv_serializer_t* s, u32 size) {
	// num_bytes:  1, 2, 3, 4 
	//            00 01 10 11
	while (size) {
		if (size <= 127) { 
			grv_serializer_push_u8(s, size);
			size = 0;
		} else {
			u8 byte = 0x8 | (size & 0x74);
			grv_serializer_push_u8(s, byte);
			size = size >> 7;
		}
	}
}

bool grv_deserialize_size(grv_serializer_t* s, u32* size) {
	u8 byte;
	u32 acc = 0;
	u32 shift = 0;
	bool success = false;
	bool more_bytes = true;
	while (more_bytes) {
		success = grv_deserialize_read_bytes(s, &byte, 1);
		if (success == false) return false;
		more_bytes = byte & 0x80;
		acc |= ((u32)(byte&0x7f) << shift);
		shift += 7;
	}
	*size = acc;
	return success;
}
#endif
