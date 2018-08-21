#ifndef __IM_LAYER_BLEND_MODE_H__
#define __IM_LAYER_BLEND_MODE_H__ 1

#include "common.h"

typedef vec3 (*blend_mode_func_t)(vec3 a, vec3 b, float opacity) ;

vec3 blend_normal(vec3 a, vec3 b, float opacity);
vec3 blend_multiply(vec3 a, vec3 b, float opacity) ;
vec3 blend_screen(vec3 a, vec3 b, float opacity) ;

#endif /* __IM_LAYER_BLEND_MODE_H__ */
