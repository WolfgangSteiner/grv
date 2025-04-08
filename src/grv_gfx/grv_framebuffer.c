#include "grv/grv_common.h"
#include "grv/grv_math.h"
#include "grv_gfx/grv_framebuffer.h"
#include <stdlib.h>
#include <assert.h>

void grv_clipping_stack_init(grv_clipping_stack_t* stack) {
    stack->stack = NULL;
    stack->capacity = 0L;
    stack->size = 0L;
}

void grv_clipping_stack_push(grv_clipping_stack_t* stack, rect_i32 rect) {
    if (stack->stack == NULL) {
        stack->capacity = 16;
        stack->stack = malloc(16 * sizeof(rect_i32));
    } else if (stack->size == stack->capacity) {
        stack->capacity *= 2;
        stack->stack = realloc(stack->stack, stack->capacity);
    }
    stack->stack[stack->size++] = rect;
}

rect_i32 grv_clipping_stack_pop(grv_clipping_stack_t* stack) {
    assert(stack->size);
    rect_i32 result = stack->stack[stack->size - 1];
    stack->size--;
    return result;
}

rect_i32* grv_clipping_stack_top(grv_clipping_stack_t* stack) {
    return stack->size ? stack->stack + stack->size - 1 : NULL;
}

void grv_framebuffer_init(grv_framebuffer_t* fb, grv_framebuffer_type_t type, i32 width, i32 height) {
    fb->type = type;
    fb->width = width;
    fb->height = height;
    fb->row_skip = width;
    fb->clear_color_u8 = 0;

    if (type == GRV_FRAMEBUFFER_INDEXED) {
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

void grv_framebuffer_clear(grv_framebuffer_t* fb) {
    u32 count = fb->width * fb->height;
    if (fb->type == GRV_FRAMEBUFFER_INDEXED) {
        grv_framebuffer_fill_u8(fb, fb->clear_color_u8);
    } else {
        memset(fb->rgba_data, 0x0, count * sizeof(u32));
    }
}

void grv_framebuffer_fill_u8(grv_framebuffer_t* fb, u8 color) {
    u8* row_ptr = fb->indexed_data;
    for (i32 y = 0; y < fb->height; ++y) {
        memset(row_ptr, color, fb->width);
        row_ptr += fb->row_skip;
    }
}

rect_i32 grv_framebuffer_get_clipping_rect(grv_framebuffer_t* fb) {
    rect_i32* current_clipping_rect = grv_clipping_stack_top(&fb->clipping_stack);
    if (current_clipping_rect) {
        return *current_clipping_rect;
    } else {
        return (rect_i32){0, 0, fb->width, fb->height};
    }
}

u8* grv_framebuffer_pixel_address_u8(grv_framebuffer_t* fb, i32 x, i32 y) {
    assert(x >= 0 && x < fb->width);
    assert(y >= 0 && y < fb->height);
    return fb->indexed_data + y * fb->row_skip + x;
}

void grv_framebuffer_clear_span_buffer(grv_framebuffer_t* fb) {
    fb->span_buffer.count = 0;
}

void grv_framebuffer_push_span(grv_framebuffer_t* fb, i32 y, i32 x1, i32 x2) {
    if (fb->span_buffer.count == 0) {
        fb->span_buffer.y_start = y;
    } 
    
    if (fb->span_buffer.count >= fb->span_buffer.capacity) {
        assert(false);
    } else {
        fb->span_buffer.spans[fb->span_buffer.count++] = (grv_span_t){grv_min_i32(x1, x2), grv_max_i32(x1, x2)};
    }
}

void grv_framebuffer_render_argb(grv_framebuffer_t* fb, u32* argb_data, i32 pitch) {
    if (fb->type == GRV_FRAMEBUFFER_INDEXED) {
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

void grv_framebuffer_set_pixel_u8(grv_framebuffer_t* fb, vec2i pos, u8 color) {
    if (fb->use_clipping) {
        rect_i32 clipping_rect = grv_framebuffer_get_clipping_rect(fb);
        if (!rect_i32_point_inside(clipping_rect, pos)) return;
    }
    u8* pixel = grv_framebuffer_pixel_address_u8(fb, pos.x, pos.y);
    *pixel = color;
}

void grv_framebuffer_set_pixel_scaled_u8(grv_framebuffer_t* fb, vec2i pos, u32 scale, u8 color) {
    rect_i32 rect = {pos.x, pos.y, scale, scale};    
    grv_framebuffer_fill_rect_u8(fb, rect, color);
}


void grv_framebuffer_fill_rect_u8(grv_framebuffer_t* fb, rect_i32 rect, u8 color) {
    if (fb->use_clipping) {
        rect_i32 clipping_rect = grv_framebuffer_get_clipping_rect(fb);
        rect = rect_i32_intersect_rect(rect, clipping_rect);
    }
    if (rect.w <= 0 || rect.h <= 0) return;

    for (i32 y = rect_i32_ymin(rect); y <= rect_i32_ymax(rect); ++y) {
        u8* pixel = grv_framebuffer_pixel_address_u8(fb, rect.x, y);
        memset(pixel, color, rect.w);
    }
}

void _grv_framebuffer_draw_horizontal_line_u8(
    grv_framebuffer_t* fb, i32 x1, i32 x2, i32 y, u8 color, rect_i32 clip_rect) {
    if (y < clip_rect.y || y >= clip_rect.y + clip_rect.h) return;
    i32 cx1 = clip_rect.x;
    i32 cx2 = clip_rect.x + clip_rect.w - 1;
    if (x1 > cx2 || x2 < cx1) return;
    i32 x = grv_clamp_i32(x1, cx1, cx2);
    x2 = grv_clamp_i32(x2, cx1, cx2);
    u8* ptr = grv_framebuffer_pixel_address_u8(fb, x, y);
    while (x <= x2) {
        *ptr++ = color;
        x++;
    }
}

void _grv_framebuffer_draw_vertical_line_u8(
    grv_framebuffer_t* fb, i32 x, i32 y1, i32 y2, u8 color, rect_i32 clip_rect) {
    if (x < clip_rect.x || x >= clip_rect.x + clip_rect.w) return;
    i32 cy1 = clip_rect.y;
    i32 cy2 = clip_rect.y + clip_rect.w - 1;
    i32 y = grv_clamp_i32(y1, cy1, cy2);
    y2 = grv_clamp_i32(y2, cy1, cy2);
    u8* ptr = grv_framebuffer_pixel_address_u8(fb, x, y);
    while (y <= y2) {
        *ptr = color;
        ptr += fb->row_skip;
        y++;
    }
}

void grv_framebuffer_fill_rect_chamfered_u8(grv_framebuffer_t* fb, rect_i32 rect, u8 color) {
	rect_i32 clipped_rect = rect;
    rect_i32 clipping_rect = grv_framebuffer_get_clipping_rect(fb);
    if (fb->use_clipping) {
        clipped_rect = rect_i32_intersect_rect(rect, clipping_rect);
    }
    if (clipped_rect.w <= 0 || clipped_rect.h <= 0) return;

    for (i32 y = rect_i32_ymin(clipped_rect) + 1; y < rect_i32_ymax(clipped_rect); ++y) {
        u8* pixel = grv_framebuffer_pixel_address_u8(fb, clipped_rect.x, y);
        memset(pixel, color, clipped_rect.w);
    }
	_grv_framebuffer_draw_horizontal_line_u8(
		fb, rect_i32_xmin(rect) + 1, rect_i32_xmax(rect) - 1, rect_i32_ymin(rect), color, clipping_rect);
	_grv_framebuffer_draw_horizontal_line_u8(
		fb, rect_i32_xmin(rect) + 1, rect_i32_xmax(rect) - 1, rect_i32_ymax(rect), color, clipping_rect);
}


void grv_framebuffer_draw_rect_u8(grv_framebuffer_t* fb, rect_i32 rect, u8 color) {
    rect_i32 clip_rect = {.x=0,.y=0,.w=fb->width,.h=fb->height};
    if (fb->use_clipping) {
        clip_rect = grv_framebuffer_get_clipping_rect(fb);
    }

    if (rect.w <= 0 || rect.h <= 0) return;

    i32 x1 = rect.x;
    i32 x2 = rect.x + rect.w - 1;
    i32 y1 = rect.y;
    i32 y2 = rect.y + rect.h - 1;

    _grv_framebuffer_draw_horizontal_line_u8(fb, x1, x2, y1, color, clip_rect);
    _grv_framebuffer_draw_horizontal_line_u8(fb, x1, x2, y2, color, clip_rect);
    _grv_framebuffer_draw_vertical_line_u8(fb, x1, y1, y2, color, clip_rect);
    _grv_framebuffer_draw_vertical_line_u8(fb, x2, y1, y2, color, clip_rect);
}

void grv_framebuffer_draw_rect_chamfered_u8(grv_framebuffer_t* fb, rect_i32 rect, u8 color) {
    rect_i32 clip_rect = {.x=0,.y=0,.w=fb->width,.h=fb->height};
    if (fb->use_clipping) {
        clip_rect = grv_framebuffer_get_clipping_rect(fb);
    }

    if (rect.w <= 0 || rect.h <= 0) return;

    i32 x1 = rect.x;
    i32 x2 = rect.x + rect.w - 1;
    i32 y1 = rect.y;
    i32 y2 = rect.y + rect.h - 1;

    _grv_framebuffer_draw_horizontal_line_u8(fb, x1+1, x2-1, y1, color, clip_rect);
    _grv_framebuffer_draw_horizontal_line_u8(fb, x1+1, x2-1, y2, color, clip_rect);
    _grv_framebuffer_draw_vertical_line_u8(fb, x1, y1+1, y2-1, color, clip_rect);
    _grv_framebuffer_draw_vertical_line_u8(fb, x2, y1+1, y2-1, color, clip_rect);
}

void grv_framebuffer_blit_img8(grv_framebuffer_t* fb, grv_img8_t* img, i32 x, i32 y) {
    i32 x_start = x;
    i32 x_end = x + img->w - 1;
    i32 y_start = y;
    i32 y_end = y + img->h - 1;
    if (x_start >= fb->width || x_end < 0 || y_start >= fb->height || y_end < 0) return;

    x_start = grv_clamp_i32(x_start, 0, fb->width - 1);
    x_end = grv_clamp_i32(x_end, 0, fb->width - 1);
    y_start = grv_clamp_i32(y_start, 0, fb->height - 1);
    y_end = grv_clamp_i32(y_end, 0, fb->height - 1);

    u8* src_row_ptr = img->pixel_data + (y_start - y) * img->row_skip + (x_start - x);
    u8* dst_row_ptr = fb->indexed_data + y_start * fb->width + x_start;
    for (i32 iy = y_start; iy <= y_end; ++iy) {
        u8* src_ptr = src_row_ptr;
        u8* dst_ptr = dst_row_ptr;
        for (i32 ix = x_start; ix <= x_end; ++ix) {
            u8 src_value = *src_ptr++;
            if (src_value > 0) *dst_ptr = src_value;
            dst_ptr++;
        }
        src_row_ptr += img->row_skip;
        dst_row_ptr += fb->width;
    }    
}

void grv_framebuffer_draw_circle_u8(grv_framebuffer_t* fb, i32 px, i32 py, i32 r, u8 color) {
    if (r <= 0) return;
    if (r == 1) {
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px,py}, color);
        return;
    }
    i32 cx = r;
    i32 cy = 0;
    i32 p = -r;
    while (cx >= cy) {
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px + cx, py + cy}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px - cx, py + cy}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px + cx, py - cy}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px - cx, py - cy}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px + cy, py + cx}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px - cy, py + cx}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px + cy, py - cx}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px - cy, py - cx}, color);
        
        cy += 1;
        if (p <= 0) {
            p += 2*cy + 1;

        } else {
            cx -= 1;
            p += 2*cy + 1 - 2*cx;
        }
    }
}

void grv_framebuffer_fill_circle_u8(grv_framebuffer_t* fb, i32 px, i32 py, i32 r, u8 color) {
    if (r <= 0) return;
    if (r == 1) {
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px,py}, color);
        return;
    }

    rect_i32 clip_rect = {.x=0,.y=0,.w=fb->width,.h=fb->height};
    if (fb->use_clipping) {
        clip_rect = grv_framebuffer_get_clipping_rect(fb);
    }

    i32 cx = r;
    i32 cy = 0;
    i32 p = -r;
    while (cx >= cy) {
        i32 y = py + cy;
        i32 x1 = px - cx;
        i32 x2 = px + cx;
        _grv_framebuffer_draw_horizontal_line_u8(fb, x1, x2, py+cy, color, clip_rect); 
        _grv_framebuffer_draw_horizontal_line_u8(fb, x1, x2, py-cy, color, clip_rect); 
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px + cy, py + cx}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px - cy, py + cx}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px + cy, py - cx}, color);
        grv_framebuffer_set_pixel_u8(fb, (vec2i){px - cy, py - cx}, color);

        cy += 1;
        if (p <= 0) {
            p += 2*cy + 1;

        } else {
            cx -= 1;
            p += 2*cy + 1 - 2*cx;
            x1 = px - cy + 1;
            x2 = px + cy - 1;
            _grv_framebuffer_draw_horizontal_line_u8(fb, x1, x2, py+cx, color, clip_rect); 
            _grv_framebuffer_draw_horizontal_line_u8(fb, x1, x2, py-cx, color, clip_rect); 
        }
    }
}
