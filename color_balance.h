#ifndef __IM_COLOR_BALANCE_H__
#define __IM_COLOR_BALANCE_H__ 1

#include "layer.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void adjust_color_balance(
  layer_t layer, 
  float cyan_red_coef[3],
  float magenta_green_coef[3],
  float yellow_blue_coef[3],
  _Bool preserve_luminosity,
  rect_t zone);

void replace_color(
  layer_t layer, 
  float reds_replacement, 
  float yellows_replacement, 
  float greens_replacement, 
  float cyans_replacement, 
  float blues_replacement, 
  float magentas_replacement, 
  rect_t zone) ;
void adjust_color_saturation(
  layer_t layer, 
  float reds_sat, float yellows_sat, float greens_sat, float cyans_sat, float blues_sat, float magentas_sat, 
  float reds_light, float yellows_light, float greens_light, float cyans_light, float blues_light, float magentas_light, 
  rect_t zone);

#ifdef __cplusplus
}
#endif

#endif /* __IM_COLOR_BALANCE_H__ */
