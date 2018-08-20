#ifndef __IM_BRUSH_H__
#define __IM_BRUSH_H__ 1

#include "common.h"
#include "layer.h"

void brush_touch(layer_t layer, draw_mode_t canvas_select, int radius, float opacity, int x, int y, vec3 color);

#endif /* __IM_BRUSH_H__ */
