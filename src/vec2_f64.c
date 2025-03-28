#include "grv/vec2_f64.h"
#include "grv/vec2i.h"
#include "grv/grv_common.h"
#include "grv/grv_math.h"



vec2_f64 vec2_f64_rot(vec2_f64 a, f64 rot) {
    f64 sinv = unit_sin_f64(rot);
    f64 cosv = unit_cos_f64(rot);
    return (vec2_f64){ cosv * a.x - sinv * a.y, sinv * a.x + cosv * a.y };
}

vec2_f64 vec2_f64_lerp(vec2_f64 a, vec2_f64 b, f64 t) {
    return vec2_f64_add(a, vec2_f64_smul(vec2_f64_sub(b, a), t));
    //return (vec2_f64){a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t};
}
