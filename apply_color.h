#ifndef __IM_APPLY_COLOR_H__
#define __IM_APPLY_COLOR_H__ 1

#include "common_types.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  float error;
  float cyan_red_coef[3];
  float magenta_green_coef[3];
  float yellow_blue_coef[3];
  float brightness, contrast, exposure, saturation;
} apply_color_params_t;


apply_color_params_t apply_color(layer_t source_layer, rect_t source_zone, layer_t destination_layer, float step, _Bool preserve_luminosity) ;

#ifdef __cplusplus
}
#endif

#endif /* __IM_APPLY_COLOR_H__ */
