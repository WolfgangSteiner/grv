#ifndef GRV_LOG_H
#define GRV_LOG_H

#include "grv/grv_str.h"

typedef enum {
    GRV_LOG_LEVEL_DEBUG,
    GRV_LOG_LEVEL_INFO,
    GRV_LOG_LEVEL_WARNING,
    GRV_LOG_LEVEL_ERROR,
} grv_log_level_t;

extern grv_log_level_t grv_log_level;

#define GRV_LOG_LEVEL_DEBUG_STR "DEBUG"
#define GRV_LOG_LEVEL_INFO_STR  "INFO" 
#define GRV_LOG_LEVEL_WARNING_STR "WARNING"
#define GRV_LOG_LEVEL_ERROR_STR "ERROR"

#ifdef __GNUC__
#define GRV_CHECK_PRINTF_FORMAT(A, B) \
    __attribute__((format(printf, A, B)))
#else
#define GRV_CHECK_PRINTF_FORMAT(A, B)
#endif

#define grv_log_debug(MSG) grv_log_debug_impl(__FILE__, __LINE__, MSG)

void grv_log(int log_level, char* log_tag, grv_str_t msg);
void grv_log_debug_impl(char* filename, int line, grv_str_t msg);

void grv_log_info(grv_str_t msg);
void grv_log_error(grv_str_t msg);
void grv_log_warning(grv_str_t msg);

#endif
