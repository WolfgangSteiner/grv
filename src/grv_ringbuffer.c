#include "grv/grv_ringbuffer.h"
#include "grv/grv_memory.h"
#include "grv/grv_common.h"

void grv_ringbuffer_init(grv_ringbuffer_t* rb, i64 buffer_size) {
	*rb = (grv_ringbuffer_t) {
		.capacity=buffer_size,
		.data=grv_alloc(buffer_size)
	};
}

void grv_ringbuffer_write(grv_ringbuffer_t* rb, void* data, i64 num_bytes) {
	i64 write_idx = atomic_load(&rb->write_idx);
	u8* read_ptr = data;
	while (num_bytes) {
		i64 i_num_bytes = grv_min_i64(rb->capacity - write_idx, num_bytes);
		memcpy(rb->data + write_idx, read_ptr, i_num_bytes);
		num_bytes -= i_num_bytes;
		read_ptr += i_num_bytes;
		write_idx = (write_idx + i_num_bytes) % rb->capacity;
	}
	atomic_store(&rb->write_idx, write_idx);
}

i64 grv_ringbuffer_read(grv_ringbuffer_t* rb, void* dst, i64 dst_size) {
	i64 bytes_read = 0;
	i64 write_idx = atomic_load(&rb->write_idx);
	i64 read_idx = atomic_load(&rb->read_idx);
	i64 bytes_available = (write_idx + rb->capacity - read_idx) % rb->capacity;
	i64 total_bytes_to_read = grv_min_i64(bytes_available, dst_size);
	i64 remaining_bytes_to_read = total_bytes_to_read;
	u8* dst_ptr = dst;
	while (remaining_bytes_to_read > 0) {
		i64 i_bytes_read = grv_min_i64(rb->capacity - read_idx, remaining_bytes_to_read);
		memcpy(dst_ptr, rb->data + read_idx, i_bytes_read);
		read_idx = (read_idx + i_bytes_read) % rb->capacity;
		dst_ptr += i_bytes_read;
		bytes_read += i_bytes_read;
		remaining_bytes_to_read -= i_bytes_read;
	}
	atomic_store(&rb->read_idx, read_idx);
	return bytes_read;
}

void grv_ringbuffer_read_to_file(grv_ringbuffer_t* rb, FILE* fp) {
	i64 write_idx = atomic_load(&rb->write_idx);
	i64 read_idx = atomic_load(&rb->read_idx);
	i64 bytes_available = (write_idx + rb->capacity - read_idx) % rb->capacity;
	i64 remaining_bytes_to_read = bytes_available;
	while (remaining_bytes_to_read > 0) {
		i64 i_bytes_read = grv_min_i64(rb->capacity - read_idx, remaining_bytes_to_read);
		fwrite(rb->data + read_idx, 1, i_bytes_read, fp);
		read_idx = (read_idx + i_bytes_read) % rb->capacity;
		remaining_bytes_to_read -= i_bytes_read;
	}
	atomic_store(&rb->read_idx, read_idx);
}
