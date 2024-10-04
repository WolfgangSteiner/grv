#ifndef GRV_RESULT_TYPE_H
#define GRV_RESULT_TYPE_H

#include "grv/grv_error.h"
#include "grv/grv_base.h"

#define GRV_RESULT_TYPE_DECL(TYPE)\
    typedef struct {\
        bool valid;\
        union {\
            TYPE value;\
            grv_error_t error;\
        };\
    } grv_##TYPE##_result_t

GRV_RESULT_TYPE_DECL(bool);
GRV_RESULT_TYPE_DECL(i32);
GRV_RESULT_TYPE_DECL(u32);
GRV_RESULT_TYPE_DECL(i64);
GRV_RESULT_TYPE_DECL(u64);
GRV_RESULT_TYPE_DECL(u8);
GRV_RESULT_TYPE_DECL(size_t);

#endif
