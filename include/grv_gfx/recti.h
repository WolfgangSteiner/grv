#ifndef GRV_RECTI_H
#define GRV_RECTI_H

#include "grv/grv_base.h"
#include "grv/vec2i.h"
#include "grv/grv_common.h"

typedef struct {
   s32 x, y, w, h;
} recti_t;

GRV_INLINE recti_t recti_from_xy(s32 x1, s32 y1, s32 x2, s32 y2) {
   return (recti_t){
      .x = x1,
      .y = y1,
      .w = x2 - x1 + 1,
      .h = y2 - y1 + 1,
   };
}

GRV_INLINE s32 recti_xmin(recti_t a) { return a.x; }
GRV_INLINE s32 recti_xmax(recti_t a) { return a.x + a.w - 1; }
GRV_INLINE s32 recti_ymin(recti_t a) { return a.y; }
GRV_INLINE s32 recti_ymax(recti_t a) { return a.y + a.h - 1; }

GRV_INLINE vec2i recti_pos(recti_t a) { return (vec2i){a.x, a.y}; }

GRV_INLINE bool grv_recti_contains_point(recti_t rect, vec2i point) {
   return point.x >= recti_xmin(rect) && point.x <= recti_xmax(rect) &&
          point.y >= recti_ymin(rect) && point.y <= recti_ymax(rect);
}

GRV_INLINE recti_t grv_recti_intersect_rect(recti_t a, recti_t b) {
   recti_t result;
   result.x = grv_max_s32(a.x, b.x);
   result.y = grv_max_s32(a.y, b.y);
   s32 xmax = grv_min_s32(recti_xmax(a), recti_xmax(b));
   s32 ymax = grv_min_s32(recti_ymax(a), recti_ymax(b));
   result.w = xmax - result.x + 1;
   result.h = ymax - result.y + 1;
   return result;
}

GRV_INLINE recti_t grv_recti_center_in_rect(recti_t rect, recti_t container) {
   return (recti_t){
      .x = container.x + (container.w - rect.w) / 2,
      .y = container.y + (container.h - rect.h) / 2,
      .w = rect.w,
      .h = rect.h,
   };
}

#endif
