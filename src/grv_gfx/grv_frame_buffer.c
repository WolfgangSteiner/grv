#include "grv/grv_common.h"
#include "grv/grv_math.h"
#include "grv_gfx/grv_frame_buffer.h"
#include <stdlib.h>
#include <assert.h>

void grv_clipping_stack_init(grv_clipping_stack_t* stack) {
    stack->stack = NULL;
    stack->capacity = 0L;
    stack->size = 0L;
}

void grv_clipping_stack_push(grv_clipping_stack_t* stack, recti_t rect) {
    if (stack->stack == NULL) {
        stack->capacity = 16;
        stack->stack = malloc(16 * sizeof(recti_t));
    } else if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        stack->stack = realloc(stack->stack, stack->capacity);
    }
    stack->stack[stack->size++] = rect;
}

recti_t grv_clipping_stack_pop(grv_clipping_stack_t* stack) {
    assert(stack->size);
    recti_t result = stack->stack[stack->size - 1];
    stack->size--;
    return result;
}

recti_t* grv_clipping_stack_top(grv_clipping_stack_t* stack) {
    return stack->size ? stack->stack + stack->size - 1 : NULL;
}

void grv_frame_buffer_init(grv_frame_buffer_t* fb, grv_frame_buffer_type_t type, i32 width, i32 height) {
    fb->type = type;
    fb->width = width;
    fb->height = height;
    fb->row_skip = width;
    fb->clear_color_u8 = 0;

    if (type == FRAME_BUFFER_INDEXED) {
        color_palette_init_grayscale(&fb->palette, 16);
        fb->indexed_data = grv_alloc_zeros(width * height * sizeof(u8));
    } else {
        fb->palette.num_entries = 0;
        fb->palette.entries = NULL;
        fb->rgba_data = grv_alloc_zeros(width * height * sizeof(u32));
    }
    fb->use_clipping = true;
    grv_clipping_stack_init(&fb->clipping_stack);
    fb->span_buffer.spans = grv_alloc_zeros(height * sizeof(grv_span_t));
    fb->span_buffer.capacity = height;
}

void grv_frame_buffer_clear(grv_frame_buffer_t* fb) {
    u32 count = fb->width * fb->height;
    if (fb->type == FRAME_BUFFER_INDEXED) {
        grv_frame_buffer_fill_u8(fb, fb->clear_color_u8);
    } else {
        memset(fb->rgba_data, 0x0, count * sizeof(u32));
    }
}

void grv_frame_buffer_fill_u8(grv_frame_buffer_t* fb, u8 color) {
    u8* row_ptr = fb->indexed_data;
    for (i32 y = 0; y < fb->height; ++y) {
        memset(row_ptr, color, fb->width);
        row_ptr += fb->row_skip;
    }
}

recti_t grv_frame_buffer_get_clipping_rect(grv_frame_buffer_t* fb) {
    recti_t* current_clipping_rect = grv_clipping_stack_top(&fb->clipping_stack);
    if (current_clipping_rect) {
        return *current_clipping_rect;
    } else {
        return (recti_t){0, 0, fb->width, fb->height};
    }
}

u8* grv_frame_buffer_pixel_address_u8(grv_frame_buffer_t* fb, i32 x, i32 y) {
    assert(x >= 0 && x < fb->width);
    assert(y >= 0 && y < fb->height);
    return fb->indexed_data + y * fb->row_skip + x;
}

void grv_frame_buffer_clear_span_buffer(grv_frame_buffer_t* fb) {
    fb->span_buffer.count = 0;
}

void grv_frame_buffer_push_span(grv_frame_buffer_t* fb, i32 y, i32 x1, i32 x2) {
    if (fb->span_buffer.count == 0) {
        fb->span_buffer.y_start = y;
    } 
    
    if (fb->span_buffer.count >= fb->span_buffer.capacity) {
        assert(false);
    } else {
        fb->span_buffer.spans[fb->span_buffer.count++] = (grv_span_t){grv_min_i32(x1, x2), grv_max_i32(x1, x2)};
    }
}

void grv_frame_buffer_render_argb(grv_frame_buffer_t* fb, u32* argb_data, i32 pitch) {
    if (fb->type == FRAME_BUFFER_INDEXED) {
        i32 num_colors = fb->palette.num_entries;
        u32* dst_row_ptr = argb_data;
        u8* src_row_ptr = fb->indexed_data;
        for (i32 y = 0; y < fb->height; ++y) {
            u32* dst_ptr = dst_row_ptr; 
            u8* src_ptr = src_row_ptr;
            for (i32 x = 0; x < fb->width; ++x) {
                u8 index = *src_ptr++;
                u32 color_rgba = 0;
                if (num_colors <= 128 && index & 0x80) {
                    color_rgba = color_palette_map(&fb->palette, index & 0x7f);
                    color_rgba = grv_rgba_invert(color_rgba);
                } else {
                    color_rgba = color_palette_map(&fb->palette, index);
                }
                *dst_ptr++ = grv_rgba_to_argb(color_rgba);
            }
            dst_row_ptr += pitch / sizeof(u32);
            src_row_ptr += fb->row_skip;
        }
    } else {
        u32* src_row_ptr = fb->rgba_data;
        u32* row_ptr = argb_data;
        for (i32 y = 0; y < fb->height; ++y) {
            u32* dst_ptr = row_ptr;
            u32* src_ptr = src_row_ptr;
            for (i32 x = 0; x < fb->width; ++x) {
                *dst_ptr++ = grv_rgba_to_argb(*src_ptr++);
            }
            row_ptr += pitch / sizeof(u32);
            src_row_ptr += fb->row_skip;
        }
    }        
}

void grv_frame_buffer_set_pixel_u8(grv_frame_buffer_t* fb, vec2i pos, u8 color) {
    if (fb->use_clipping) {
        recti_t clipping_rect = grv_frame_buffer_get_clipping_rect(fb);
        if (!grv_recti_contains_point(clipping_rect, pos)) return;
    }
    u8* pixel = grv_frame_buffer_pixel_address_u8(fb, pos.x, pos.y);
    *pixel = color;
}

void grv_frame_buffer_set_pixel_scaled_u8(grv_frame_buffer_t* fb, vec2i pos, u32 scale, u8 color) {
    recti_t rect = {pos.x, pos.y, scale, scale};    
    grv_frame_buffer_fill_rect_u8(fb, rect, color);
}


void grv_frame_buffer_fill_rect_u8(grv_frame_buffer_t* fb, recti_t rect, u8 color) {
    if (fb->use_clipping) {
        recti_t clipping_rect = grv_frame_buffer_get_clipping_rect(fb);
        rect = grv_recti_intersect_rect(rect, clipping_rect);
    }
    if (rect.w <= 0 || rect.h <= 0) return;

    for (i32 y = recti_ymin(rect); y <= recti_ymax(rect); ++y) {
        u8* pixel = grv_frame_buffer_pixel_address_u8(fb, rect.x, y);
        memset(pixel, color, rect.w);
    }
}
