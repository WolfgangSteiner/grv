#ifndef GRV_WINDOW_H
#define GRV_WINDOW_H

#include "grv/grv_base.h"
#include "grv/grv_str.h"
#include "grv_frame_buffer.h"
#include "grv/grv_arr.h"

typedef struct {
    i32 width, height;
    f32 scale;
    grv_str_t title;
    grv_frame_buffer_t frame_buffer;
    bool should_close;
    bool borderless;
    bool use_int_scaling;
    void* handle;
} grv_window_t;

grv_window_t* grv_window_new(i32 width, i32 height, f32 scale, grv_str_t title);
bool grv_window_show(grv_window_t* w);

GRV_ARR_TYPEDEF(grv_window_t*) grv_window_active_windows_arr_t;
extern grv_window_active_windows_arr_t grv_window_active_windows_arr;

void grv_window_poll_events(void);
void grv_window_present(grv_window_t* w);
void grv_sleep(f32 seconds);
void grv_window_set_opacity(grv_window_t* w, f32 opacity);

#endif

