#ifndef __IM_COLORIZE_H__
#define __IM_COLORIZE_H__  1

#include "layer.h"

void colorize(layer_t layer, vec3 hueRGB, float saturation, float lightness, levels_t affect_levels, _Bool preserve_original_pixel_saturation, rect_t zone);

#endif /* __IM_COLORIZE_H__ */
