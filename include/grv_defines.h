#ifndef GRV_DEFINES_H
#define GRV_DEFINES_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;
typedef uint64_t u64;
typedef int64_t s64;
typedef float f32;
typedef double f64;

#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b; })
#define MAX(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a > _b ? _a : _b; })

#endif
