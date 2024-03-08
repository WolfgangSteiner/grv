#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "grv/grv_base.h"
#include "grv_gfx/grv_color_palette.h"
#include "grv_gfx/recti.h"
#include "grv/vec2i.h"

typedef enum {
    FRAME_BUFFER_INDEXED,
    FRAME_BUFFER_RGBA,
} grv_frame_buffer_type_t;

typedef struct {
    i32 x1, x2;
} grv_span_t;

typedef struct {
    grv_span_t* spans;
    u32 capacity;
    u32 count;
    i32 y_start;
} grv_span_buffer_t;

typedef struct {
    recti_t* stack;
    u32 capacity;
    u32 size;
} grv_clipping_stack_t;

void grv_clipping_stack_init(grv_clipping_stack_t* stack);
void grv_clipping_stack_push(grv_clipping_stack_t* stack, recti_t rect);
recti_t grv_clipping_stack_pop(grv_clipping_stack_t* stack);
recti_t* grv_clipping_stack_top(grv_clipping_stack_t* stack);

typedef struct {
    grv_frame_buffer_type_t type;
    i32 width, height;
    color_palette_t palette;
    u8 clear_color_u8;
    u8* indexed_data;
    u32* rgba_data;
    bool use_clipping;
    grv_clipping_stack_t clipping_stack;
    grv_span_buffer_t span_buffer;
} grv_frame_buffer_t;

void grv_frame_buffer_init(grv_frame_buffer_t* frame_buffer, grv_frame_buffer_type_t type, i32 width, i32 height);
void grv_frame_buffer_clear(grv_frame_buffer_t* frame_buffer);
recti_t grv_frame_buffer_get_clipping_rect(grv_frame_buffer_t* fb);
u8* grv_frame_buffer_pixel_address_u8(grv_frame_buffer_t* fb, i32 x, i32 y);
void grv_frame_buffer_clear_span_buffer(grv_frame_buffer_t* fb);
void grv_frame_buffer_push_span(grv_frame_buffer_t* fb, i32 y, i32 x_start, i32 x_end);
void grv_frame_buffer_render_argb(grv_frame_buffer_t* fb, u32* rgba_data, i32 pitch);
void grv_frame_buffer_set_pixel_u8(grv_frame_buffer_t* fb, vec2i pos, u8 color);
void grv_frame_buffer_set_pixel_scaled_u8(grv_frame_buffer_t* fb, vec2i pos, u32 scale, u8 color);
void grv_frame_buffer_fill_rect_u8(grv_frame_buffer_t* fb, recti_t rect, u8 color);
#endif
