#ifndef __IM_SHAPES_H__
#define __IM_SHAPES_H__ 1

#include "layer.h"
#include "common.h"
#include "layer_modes.h"


#ifdef __cplusplus
extern "C" {
#endif

void draw_gauss(layer_t layer, vec3 color, float opacity, rect_t zone, float sigma, blend_mode_func_t blend_func);
void draw_circle(layer_t layer, vec3 color, float opacity, int x, int y, int r, blend_mode_func_t b);
void draw_pixel(layer_t layer, vec3 color, float opacity, int x, int y, blend_mode_func_t b);
void draw_rect(layer_t layer, vec3 color, float opacity, rect_t rect, blend_mode_func_t b);
vec3 get_pixel(layer_t layer, int x, int y);
void draw_filled_rect(layer_t layer, vec3 color, float opacity, rect_t rect, blend_mode_func_t blend_func);
void draw_cross(layer_t layer, vec3 color, float opacity, int width, int cx, int cy, int w, int h);

#ifdef __cplusplus
}
#endif

#endif /* __IM_SHAPES_H__ */
