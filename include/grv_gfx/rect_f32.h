#ifndef GRV_rect_f32_H
#define GRV_rect_f32_H

#include "grv/grv_base.h"
#include "grv/vec2i.h"
#include "grv/grv_common.h"
#include "grv/vec2f.h"

typedef struct {
   f32 x, y, w, h;
} rect_f32_t;

GRV_INLINE rect_f32_t rect_f32_from_xy(f32 x1, f32 y1, f32 x2, f32 y2) {
   return (rect_f32_t){
      .x = x1,
      .y = y1,
      .w = x2 - x1 + 1,
      .h = y2 - y1 + 1,
   };
}

GRV_INLINE f32 rect_f32_xmin(rect_f32_t a) { return a.x; }
GRV_INLINE f32 rect_f32_xmax(rect_f32_t a) { return a.x + a.w - 1; }
GRV_INLINE f32 rect_f32_ymin(rect_f32_t a) { return a.y; }
GRV_INLINE f32 rect_f32_ymax(rect_f32_t a) { return a.y + a.h - 1; }

GRV_INLINE vec2i rect_f32_pos(rect_f32_t a) { return (vec2i){a.x, a.y}; }

GRV_INLINE bool grv_rect_f32_contains_point(rect_f32_t rect, vec2i point) {
   return point.x >= rect_f32_xmin(rect) && point.x <= rect_f32_xmax(rect) &&
          point.y >= rect_f32_ymin(rect) && point.y <= rect_f32_ymax(rect);
}

GRV_INLINE rect_f32_t grv_rect_f32_intersect_rect(rect_f32_t a, rect_f32_t b) {
   rect_f32_t result;
   result.x = grv_max_f32(a.x, b.x);
   result.y = grv_max_f32(a.y, b.y);
   f32 xmax = grv_min_f32(rect_f32_xmax(a), rect_f32_xmax(b));
   f32 ymax = grv_min_f32(rect_f32_ymax(a), rect_f32_ymax(b));
   result.w = xmax - result.x + 1;
   result.h = ymax - result.y + 1;
   return result;
}

GRV_INLINE rect_f32_t grv_rect_f32_center_in_rect(rect_f32_t rect, rect_f32_t container) {
   return (rect_f32_t){
      .x = container.x + (container.w - rect.w) / 2,
      .y = container.y + (container.h - rect.h) / 2,
      .w = rect.w,
      .h = rect.h,
   };
}

GRV_INLINE rect_f32_t grv_rect_f32_move_to(rect_f32_t rect, vec2f pos) {
   return (rect_f32_t){
      .x = pos.x,
      .y = pos.y,
      .w = rect.w,
      .h = rect.h,
   };
}

GRV_INLINE bool grv_rect_f32_point_inside(rect_f32_t r, vec2f p) {
    return p.x >= r.x
        && p.x <= r.x + r.w
        && p.y >= r.y
        && p.y <= r.y + r.h;
}


#endif
