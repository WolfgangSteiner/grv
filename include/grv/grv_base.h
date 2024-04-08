#ifndef GRV_BASE_H
#define GRV_BASE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define GRV_INLINE static inline
#define GRV_UNUSED(X) (void)(X)
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX 
#define U32_MAX UINT32_MAX
#define I8_MIN INT8_MIN
#define I8_MAX INT8_MAX
#define I16_MIN INT16_MIN
#define I16_MAX INT16_MAX
#define I32_MIN INT32_MIN
#define I32_MAX INT32_MAX
#define U64_MAX UINT64_MAX
#define I64_MAX INT64_MAX

GRV_INLINE u8 grv_max_value_u8(void) { return U8_MAX; }
GRV_INLINE u16 grv_max_value_u16(void) { return U16_MAX; }
GRV_INLINE u32 grv_max_value_u32(void) { return U32_MAX; }
GRV_INLINE u64 grv_max_value_u64(void) { return U64_MAX; }
GRV_INLINE i8 grv_max_value_i8(void) { return I8_MAX; }
GRV_INLINE i16 grv_max_value_i16(void) { return I16_MAX; }
GRV_INLINE i32 grv_max_value_i32(void) { return I32_MAX; }
GRV_INLINE i64 grv_max_value_i64(void) { return I64_MAX; }

#define GRV_MAX_VALUE(VALUE) _Generic((VALUE), \
    u8: grv_max_value_u8, \
    u16: grv_max_value_u16, \
    u32: grv_max_value_u32, \
    u64: grv_max_value_u64, \
    i8: grv_max_value_i8, \
    i16: grv_max_value_i16, \
    i32: grv_max_value_i32, \
    i64: grv_max_value_i64 \
)()

#define grv_assert(COND) assert(COND)
#define GRV_NOT_IMPLEMENTED() assert(false)

#ifdef GRV_DEBUG
    #define grv_debug_assert(COND) grv_assert(COND)
#else
    #define grv_debug_assert(COND)
#endif

typedef enum {
    GRV_ERROR_SUCCESS,
    GRV_ERROR_OTHER,
    GRV_ERROR_FILE_NOT_FOUND,
    GRV_ERROR_FILE_NOT_READABLE,
    GRV_ERROR_FILE_NOT_WRITABLE,
    GRV_ERROR_DIRECTORY_NOT_READABLE,
    GRV_ERROR_PATH_EMPTY,
    GRV_ERROR_PATH_EXISTS,
    GRV_ERROR_CREATING_DIRECTORY,
    GRV_ERROR_PERMISSION_DENIED,
    GRV_ERROR_FILE_LOCKED,
    GRV_ERROR_INVALID_KEY,
} grv_error_t;



#endif
