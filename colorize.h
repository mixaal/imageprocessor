#ifndef __IM_COLORIZE_H__
#define __IM_COLORIZE_H__  1

#include <stdbool.h>
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void colorize(layer_t layer, vec3 hueRGB, float saturation, float lightness, levels_t affect_levels, _Bool preserve_original_pixel_saturation, rect_t zone);

#ifdef __cplusplus
}
#endif

#endif /* __IM_COLORIZE_H__ */
