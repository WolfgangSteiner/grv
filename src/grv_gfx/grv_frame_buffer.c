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

void grv_frame_buffer_init(grv_frame_buffer_t* fb, grv_frame_buffer_type_t type, u32 width, u32 height) {
    fb->type = type;
    fb->width = width;
    fb->height = height;

    if (type == FRAME_BUFFER_INDEXED) {
        color_palette_init_grayscale(&fb->palette, 16);
        fb->indexed_data = grv_alloc_zeros(width * height * sizeof(u8));
    } else {
        fb->palette.num_entries = 0;
        fb->palette.entries = NULL;
        fb->rgba_data = grv_alloc_zeros(width * height * sizeof(color_rgba_t));
    }
    fb->use_clipping = true;
    grv_clipping_stack_init(&fb->clipping_stack);
    fb->span_buffer.spans = grv_alloc_zeros(height * sizeof(grv_span_t));
    fb->span_buffer.capacity = height;
}

void grv_frame_buffer_clear(grv_frame_buffer_t* frame_buffer) {
    u32 count = frame_buffer->width * frame_buffer->height;

    if (frame_buffer->type == FRAME_BUFFER_INDEXED) {
        memset(frame_buffer->indexed_data, 0x0, count * sizeof(u8));
    } else {
        memset(frame_buffer->rgba_data, 0x0, count * sizeof(color_rgba_t));
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

u8* grv_frame_buffer_pixel_address_u8(grv_frame_buffer_t* fb, s32 x, s32 y) {
    assert(x >= 0 && (u32)x < fb->width);
    assert(y >= 0 && (u32)y < fb->height);
    return fb->indexed_data + y * fb->width + x;
}

void grv_frame_buffer_clear_span_buffer(grv_frame_buffer_t* fb) {
    fb->span_buffer.count = 0;
}

void grv_frame_buffer_push_span(grv_frame_buffer_t* fb, s32 y, s32 x1, s32 x2) {
    if (fb->span_buffer.count == 0) {
        fb->span_buffer.y_start = y;
    } 
    
    if (fb->span_buffer.count >= fb->span_buffer.capacity) {
        assert(false);
    } else {
        fb->span_buffer.spans[fb->span_buffer.count++] = (grv_span_t){min_s32(x1, x2), max_s32(x1, x2)};
    }
}

void grv_frame_buffer_render(grv_frame_buffer_t* fb, u32* rgba_data, s32 pitch) {
    if (fb->type == FRAME_BUFFER_INDEXED) {
        for (s32 y = 0; y < fb->height; ++y) {
            for (s32 x = 0; x < fb->width; ++x) {
                u8 index = fb->indexed_data[y * fb->width + x];
                color_rgba_t color = color_palette_map(&fb->palette, index);
                char* dst_ptr = (char*)rgba_data + y * pitch + x * sizeof(u32);
                (*(u32*)dst_ptr) = color.rgba;
            }
        }
    } else {
        assert(pitch == fb->width * sizeof(u32));
        memcpy(rgba_data, fb->rgba_data, fb->width * fb->height * sizeof(u32));
    }        
}