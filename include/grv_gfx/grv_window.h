#ifndef GRV_WINDOW_H
#define GRV_WINDOW_H

#include "grv/grv_base.h"
#include "grv/grv_str.h"
#include "grv_framebuffer.h"
#include "grv/grv_arr.h"
#include "grv/vec2i.h"
#include "grv/vec2f.h"
#include "grv_gfx/rect_i32.h"

typedef vec2i vec2_i32;
typedef vec2f vec2_f32;

typedef enum {
    GRV_WINDOW_HORIZONTAL_ALIGN_CENTER,
    GRV_WINDOW_HORIZONTAL_ALIGN_LEFT,
    GRV_WINDOW_HORIZONTAL_ALIGN_RIGHT,
    GRV_WINDOW_HORIZONTAL_ALIGN_NONE
} grv_window_horizontal_align_t;

typedef enum {
    GRV_WINDOW_VERTICAL_ALIGN_CENTER,
    GRV_WINDOW_VERTICAL_ALIGN_TOP,
    GRV_WINDOW_VERTICAL_ALIGN_BOTTOM,
    GRV_WINDOW_VERTICAL_ALIGN_NONE
} grv_window_vertical_align_t;


typedef struct {
	vec2_i32 initial_window_pos;
	vec2_f32 initial_view_pos;
	i32 click_count;
	bool is_down;
	bool was_down;
} grv_mouse_button_info_t;


#define GRV_WINDOW_MAX_MOUSE_BUTTON_ID 3

typedef struct {
    i32 width, height;
    grv_window_horizontal_align_t horizontal_align;
    grv_window_vertical_align_t vertical_align;
    f32 scale;
    grv_str_t title;
    grv_framebuffer_t framebuffer;
    bool should_close;
    bool borderless;
    bool resizable;
    bool use_int_scaling;
    void* handle;
	vec2_i32 mouse_window_pos;
	vec2_f32 mouse_view_pos;
	bool is_in_drag;
	vec2_i32 mouse_drag_initial_window_pos;
	vec2_f32 mouse_drag_initial_view_pos;
	grv_mouse_button_info_t mouse_button_info[GRV_WINDOW_MAX_MOUSE_BUTTON_ID + 1];
	rect_i32 view_rect;
	f32 view_scale;
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

