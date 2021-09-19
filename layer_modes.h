#ifndef __IM_LAYER_BLEND_MODE_H__
#define __IM_LAYER_BLEND_MODE_H__ 1

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef vec3 (*blend_mode_func_t)(vec3 a, vec3 b, float opacity) ;

vec3 blend_linear_light(vec3 a, vec3 b, float opacity);
vec3 blend_difference(vec3 a, vec3 b, float opacity);
vec3 blend_lighten_only(vec3 a, vec3 b, float opacity);
vec3 blend_darken_only(vec3 a, vec3 b, float opacity);
vec3 blend_substraction(vec3 a, vec3 b, float opacity);
vec3 blend_addition(vec3 a, vec3 b, float opacity);
vec3 blend_overlay(vec3 a, vec3 b, float opacity);
vec3 blend_normal(vec3 a, vec3 b, float opacity);
vec3 blend_multiply(vec3 a, vec3 b, float opacity) ;
vec3 blend_screen(vec3 a, vec3 b, float opacity) ;
vec3 blend_color_dodge(vec3 a, vec3 b, float opacity);


#ifdef __cplusplus
}
#endif

#endif /* __IM_LAYER_BLEND_MODE_H__ */
