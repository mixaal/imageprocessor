#ifndef __IM_BRUSH_H__
#define __IM_BRUSH_H__ 1


#include "common.h"
#include "layer.h"
#include "layer_modes.h"

#ifdef __cplusplus
extern "C" {
#endif

void brush_touch(layer_t layer, draw_mode_t canvas_select, int radius, float opacity, int x, int y, vec3 color, blend_mode_func_t blend_func);

#ifdef __cplusplus
}
#endif

#endif /* __IM_BRUSH_H__ */
