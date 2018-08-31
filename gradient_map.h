#ifndef __IM_GRADIENT_MAP_H__
#define __IM_GRADIENT_MAP_H__  1

#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void gradient_map(
  layer_t layer, 
  vec3 start_color,
  vec3 end_color,
  float weight, /* non-linear gradient between start/end color - 0.5f means linear */
  float opacity,
  rect_t zone);



#ifdef __cplusplus
}
#endif

#endif /* __IM_GRADIENT_MAP_H__ */
