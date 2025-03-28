#ifndef GRV_rect_f64_H
#define GRV_rect_f64_H

#include "grv/grv_base.h"
#include "grv/vec2i.h"
#include "grv/grv_common.h"
#include "grv/vec2_f64.h"

typedef struct {
   f64 x, y, w, h;
} rect_f64_t;

GRV_INLINE rect_f64_t rect_f64_from_xy(f64 x1, f64 y1, f64 x2, f64 y2) {
   return (rect_f64_t){
      .x = x1,
      .y = y1,
      .w = x2 - x1 + 1,
      .h = y2 - y1 + 1,
   };
}

GRV_INLINE f64 rect_f64_xmin(rect_f64_t a) { return a.x; }
GRV_INLINE f64 rect_f64_xmax(rect_f64_t a) { return a.x + a.w - 1; }
GRV_INLINE f64 rect_f64_ymin(rect_f64_t a) { return a.y; }
GRV_INLINE f64 rect_f64_ymax(rect_f64_t a) { return a.y + a.h - 1; }

GRV_INLINE vec2i rect_f64_pos(rect_f64_t a) { return (vec2i){a.x, a.y}; }

GRV_INLINE bool grv_rect_f64_contains_point(rect_f64_t rect, vec2i point) {
   return point.x >= rect_f64_xmin(rect) && point.x <= rect_f64_xmax(rect) &&
          point.y >= rect_f64_ymin(rect) && point.y <= rect_f64_ymax(rect);
}

GRV_INLINE rect_f64_t grv_rect_f64_intersect_rect(rect_f64_t a, rect_f64_t b) {
   rect_f64_t result;
   result.x = grv_max_f64(a.x, b.x);
   result.y = grv_max_f64(a.y, b.y);
   f64 xmax = grv_min_f64(rect_f64_xmax(a), rect_f64_xmax(b));
   f64 ymax = grv_min_f64(rect_f64_ymax(a), rect_f64_ymax(b));
   result.w = xmax - result.x + 1;
   result.h = ymax - result.y + 1;
   return result;
}

GRV_INLINE rect_f64_t grv_rect_f64_center_in_rect(rect_f64_t rect, rect_f64_t container) {
   return (rect_f64_t){
      .x = container.x + (container.w - rect.w) / 2,
      .y = container.y + (container.h - rect.h) / 2,
      .w = rect.w,
      .h = rect.h,
   };
}

GRV_INLINE rect_f64_t grv_rect_f64_move_to(rect_f64_t rect, vec2_f64 pos) {
   return (rect_f64_t){
      .x = pos.x,
      .y = pos.y,
      .w = rect.w,
      .h = rect.h,
   };
}

GRV_INLINE bool grv_rect_f64_point_inside(rect_f64_t r, vec2_f64 p) {
    return p.x >= r.x
        && p.x <= r.x + r.w
        && p.y >= r.y
        && p.y <= r.y + r.h;
}


#endif
