#ifndef GRV_WINDOW_H
#define GRV_WINDOW_H

#include "grv/grv_base.h"
#include "grv/grv_str.h"
#include "grv_frame_buffer.h"

typedef struct {
    s32 width, height;
    grv_str_t title;
    grv_frame_buffer_t frame_buffer;
    bool should_close;
    void* handle;
} grv_window_t;

grv_window_t* grv_window_new(s32 width, s32 height, grv_str_t title);
bool grv_window_show(grv_window_t* w);

typedef struct {
    grv_window_t** arr;
    s32 size;
    s32 capacity;
} grv_window_active_windows_arr_t;

extern grv_window_active_windows_arr_t grv_window_active_windows_arr;

void grv_window_poll_events();
void grv_window_present(grv_window_t* w);

#endif