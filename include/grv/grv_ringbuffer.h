#ifndef GRV_RINGBUFFER_H
#define GRV_RINGBUFFER_H

#include <stdatomic.h>
#include "grv/grv_base.h"
#include <stdio.h>

typedef struct {
	i64 capacity;
	_Atomic i64 write_idx;
	_Atomic i64 read_idx;
	u8* data;
} grv_ringbuffer_t;

void grv_ringbuffer_init(grv_ringbuffer_t* rb, i64 buffer_size);
void grv_ringbuffer_write(grv_ringbuffer_t* rb, void* data, i64 num_bytes);
i64 grv_ringbuffer_read(grv_ringbuffer_t* rb, void* dst, i64 dst_size);
void grv_ringbuffer_read_to_file(grv_ringbuffer_t* rb, FILE* fp);

#endif
