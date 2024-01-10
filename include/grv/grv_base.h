#ifndef GRV_BASE_H
#define GRV_BASE_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#define GRV_INLINE static inline

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

#define U8_MAX UINT8_MAX
#define U16_MAX UINT16_MAX 
#define U32_MAX UINT32_MAX
#define S8_MIN INT8_MIN
#define S8_MAX INT8_MAX
#define S16_MIN INT16_MIN
#define S16_MAX INT16_MAX
#define S32_MIN INT32_MIN
#define S32_MAX INT32_MAX
#define U64_MAX UINT64_MAX
#define S64_MAX INT64_MAX

GRV_INLINE u8 grv_max_value_u8() { return U8_MAX; }
GRV_INLINE u16 grv_max_value_u16() { return U16_MAX; }
GRV_INLINE u32 grv_max_value_u32() { return U32_MAX; }
GRV_INLINE u64 grv_max_value_u64() { return U64_MAX; }
GRV_INLINE s8 grv_max_value_s8() { return S8_MAX; }
GRV_INLINE s16 grv_max_value_s16() { return S16_MAX; }
GRV_INLINE s32 grv_max_value_s32() { return S32_MAX; }
GRV_INLINE s64 grv_max_value_s64() { return S64_MAX; }

#define GRV_MAX_VALUE(VALUE) _Generic((VALUE), \
    u8: grv_max_value_u8, \
    u16: grv_max_value_u16, \
    u32: grv_max_value_u32, \
    u64: grv_max_value_u64, \
    s8: grv_max_value_s8, \
    s16: grv_max_value_s16, \
    s32: grv_max_value_s32, \
    s64: grv_max_value_s64 \
)()

#define grv_assert(COND) assert(COND)

#ifdef GRV_DEBUG
    #define grv_debug_assert(COND) grv_assert(COND)
#else
    #define grv_debug_assert(COND)
#endif

#endif
