#ifndef GRV_FRAMEBUFFER_H
#define GRV_FRAMEBUFFER_H

#include "grv/grv_base.h"
#include "grv_gfx/grv_color_palette.h"
#include "grv_gfx/recti.h"
#include "grv/vec2i.h"
#include "grv_gfx/grv_img8.h"

typedef enum {
    GRV_FRAMEBUFFER_INDEXED,
    GRV_FRAMEBUFFER_RGBA,
} grv_framebuffer_type_t;

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
    grv_framebuffer_type_t type;
    i32 width, height, row_skip;
    color_palette_t palette;
    u8 clear_color_u8;
    u8* indexed_data;
    u32* rgba_data;
    bool use_clipping;
    grv_clipping_stack_t clipping_stack;
    grv_span_buffer_t span_buffer;
} grv_framebuffer_t;

void grv_framebuffer_init(grv_framebuffer_t* framebuffer, grv_framebuffer_type_t type, i32 width, i32 height);
void grv_framebuffer_clear(grv_framebuffer_t* framebuffer);
void grv_framebuffer_fill_u8(grv_framebuffer_t* framebuffer, u8 color);
recti_t grv_framebuffer_get_clipping_rect(grv_framebuffer_t* fb);
u8* grv_framebuffer_pixel_address_u8(grv_framebuffer_t* fb, i32 x, i32 y);
void grv_framebuffer_clear_span_buffer(grv_framebuffer_t* fb);
void grv_framebuffer_push_span(grv_framebuffer_t* fb, i32 y, i32 x_start, i32 x_end);
void grv_framebuffer_render_argb(grv_framebuffer_t* fb, u32* rgba_data, i32 pitch);
void grv_framebuffer_set_pixel_u8(grv_framebuffer_t* fb, vec2i pos, u8 color);
void grv_framebuffer_set_pixel_scaled_u8(grv_framebuffer_t* fb, vec2i pos, u32 scale, u8 color);
void grv_framebuffer_fill_rect_u8(grv_framebuffer_t* fb, recti_t rect, u8 color);
void grv_framebuffer_draw_rect_u8(grv_framebuffer_t* fb, recti_t rect, u8 color);
void grv_framebuffer_blit_img8(grv_framebuffer_t* fb, grv_img8_t* img, i32 x, i32 y);

#endif
