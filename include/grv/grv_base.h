#ifndef GRV_BASE_H
#define GRV_BASE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <float.h>

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

#define GRV_MAX_U8 UINT8_MAX
#define GRV_MAX_U16 UINT16_MAX 
#define GRV_MAX_U32 UINT32_MAX
#define GRV_MAX_U64 UINT64_MAX
#define GRV_MIN_I8 INT8_MIN
#define GRV_MAX_I8 INT8_MAX
#define GRV_MIN_I16 INT16_MIN
#define GRV_MAX_I16 INT16_MAX
#define GRV_MIN_I32 INT32_MIN
#define GRV_MAX_I32 INT32_MAX
#define GRV_MIN_I64 INT64_MIN
#define GRV_MAX_I64 INT64_MAX
#define GRV_MAX_F32 FLT_MAX

GRV_INLINE u8 grv_max_value_u8(void) { return GRV_MAX_U8; }
GRV_INLINE u16 grv_max_value_u16(void) { return GRV_MAX_U16; }
GRV_INLINE u32 grv_max_value_u32(void) { return GRV_MAX_U32; }
GRV_INLINE u64 grv_max_value_u64(void) { return GRV_MAX_U64; }
GRV_INLINE i8 grv_max_value_i8(void) { return GRV_MAX_I8; }
GRV_INLINE i16 grv_max_value_i16(void) { return GRV_MAX_I16; }
GRV_INLINE i32 grv_max_value_i32(void) { return GRV_MAX_I32; }
GRV_INLINE i64 grv_max_value_i64(void) { return GRV_MAX_I64; }

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
#define GRV_INVALID() assert(false);

#ifdef GRV_DEBUG
    #define grv_debug_assert(COND) grv_assert(COND)
#else
    #define grv_debug_assert(COND)
#endif

#endif
