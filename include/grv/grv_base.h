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

GRV_INLINE u8 grv_max_value_u8() { return U8_MAX; }
GRV_INLINE u16 grv_max_value_u16() { return U16_MAX; }
GRV_INLINE u32 grv_max_value_u32() { return U32_MAX; }
GRV_INLINE u64 grv_max_value_u64() { return U64_MAX; }
GRV_INLINE i8 grv_max_value_i8() { return I8_MAX; }
GRV_INLINE i16 grv_max_value_i16() { return I16_MAX; }
GRV_INLINE i32 grv_max_value_i32() { return I32_MAX; }
GRV_INLINE i64 grv_max_value_i64() { return I64_MAX; }

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

#ifdef GRV_DEBUG
    #define grv_debug_assert(COND) grv_assert(COND)
#else
    #define grv_debug_assert(COND)
#endif

#endif
