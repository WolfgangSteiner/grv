#include "grv/grv_log.h"
#include "grv/grv_str.h"
#include <stdio.h>
#include <stdarg.h>

grv_log_level_t grv_log_level = GRV_LOG_LEVEL_INFO;

void grv_log_debug_impl(char* filename, int line, grv_str_t msg) {
    if (GRV_LOG_LEVEL_DEBUG < grv_log_level) return;
    grv_str_t str = grv_str_new_with_format("[%s] {%s:%d} ", GRV_LOG_LEVEL_DEBUG_STR, filename, line);
    grv_str_append(&str, msg);
    grv_str_append_newline(&str);
    grv_str_print(str);
    grv_str_free(&str);
}

void grv_log(int log_level, char* log_tag, grv_str_t msg) {
    if (log_level < (int)grv_log_level) return;
    grv_str_t str = grv_str_new_with_format("[%s] ", log_tag);
    grv_str_append(&str, msg);
    grv_str_print(str);
    grv_str_free(&str);
}

void grv_log_info(grv_str_t msg) {
    grv_log(GRV_LOG_LEVEL_INFO, GRV_LOG_LEVEL_INFO_STR, msg);
}

void grv_log_error(grv_str_t msg) {
    grv_log(GRV_LOG_LEVEL_ERROR, GRV_LOG_LEVEL_ERROR_STR, msg);
}

void grv_log_warning(grv_str_t msg) {
    grv_log(GRV_LOG_LEVEL_WARNING, GRV_LOG_LEVEL_WARNING_STR, msg);
}
