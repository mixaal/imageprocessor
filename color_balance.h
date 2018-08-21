#ifndef __IM_COLOR_BALANCE_H__
#define __IM_COLOR_BALANCE_H__ 1

#include "layer.h"
#include "common.h"

void adjust_color_balance(
  layer_t layer, 
  float cyan_red_coef[3],
  float magenta_green_coef[3],
  float yellow_blue_coef[3],
  rect_t zone);


void adjust_color_saturation(
  layer_t layer, 
  float reds_sat, float yellows_sat, float greens_sat, float cyans_sat, float blues_sat, float magentas_sat, 
  float reds_light, float yellows_light, float greens_light, float cyans_light, float blues_light, float magentas_light, 
  rect_t zone);
#endif /* __IM_COLOR_BALANCE_H__ */
