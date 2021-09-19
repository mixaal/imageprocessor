#ifndef __IM_REINHARD_2001_H__
#define __IM_REINHARD_2001_H__ 1

#include "common_types.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif
struct mean_and_variance {
  float L_m, L_s, a_m, a_s, b_m, b_s;
};

typedef enum {
 NONE, DARKEN_ONLY, LIGHTEN_ONLY
} filter_t; 

void apply_color_reinhard2001(layer_t source, layer_t dest, rect_t source_zone, rect_t dest_zone, float variance_coef, float mean_coef, filter_t filter);
void apply_reinhard2001(layer_t source, layer_t dest, rect_t source_zone, rect_t dest_zone, struct mean_and_variance src_stat, struct mean_and_variance dst_stat, float variance_coef, float mean_coef, filter_t filter);
struct mean_and_variance compute_mean_and_variance(layer_t layer, rect_t zone);


//static struct mean_and_variance Sunny_Forest_Low_Contrast = {
//	0.774823, 0.314333, 0.237214, 0.069790, 0.012967, 0.004463
//};

//static struct mean_and_variance Dark_Forest_Low_Contrast = {
//	0.227128, 0.170400, 0.001011, 0.025360, -0.001308, 0.002711
//};

#ifdef __cplusplus
}
#endif
#endif  /* __IM_REINHARD_2001_H__ */
