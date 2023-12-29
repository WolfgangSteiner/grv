#include "grv/log.h
#include <stdio.h>
#include <stdarg.h>

grv_log_level_t grv_log_level = GRV_LOG_LEVEL_INFO;

void grv_log_debug_impl(char* filename, int line, char* format, ...) {
    if (GRV_LOG_LEVEL_DEBUG < grv_log_level) return;
    va_list args;
    va_start(args, format);
    printf("[%s] {%s:%d} ", GRV_LOG_LEVEL_DEBUG_STR, filename, line);
    vprintf(format, args);
    printf("\n");
}

void grv_log(int log_level, char* log_tag, char* format, ...) {
    if (log_level < grv_log_level) return;
    va_list args;
    va_start(args, format);
    printf("[%s] ", log_tag);
    vprintf(format, args);
    printf("\n");
}

void grv_log_info(char* format, ...) {
    va_list args;
    va_start(args, format);
    grv_log(GRV_LOG_LEVEL_INFO, GRV_LOG_LEVEL_INFO_STR, format, args);
}

void grv_log_error(char* format, ...) {
    va_list args;
    va_start(args, format);
    grv_log(GRV_LOG_LEVEL_ERROR, GRV_LOG_LEVEL_ERROR_STR, format, args);
}

void grv_log_warning(char* format, ...) {
    va_list args;
    va_start(args, format);
    grv_log(GRV_LOG_LEVEL_WARNING, GRV_LOG_LEVEL_WARNING_STR, format, args);
}