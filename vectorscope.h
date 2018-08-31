#ifndef __IM_VECTORSCOPE_H__
#define __IM_VECTORSCOPE_H__ 1

#define HK 10
#define SK 5
#define HUE_BINS (HK*360)
#define SATURATION_BINS (SK*100)

#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef float vectorscope_t[HUE_BINS][SATURATION_BINS];
void vectorscope(layer_t layer, vectorscope_t out, int *max_bin_H, int *max_bin_S, rect_t zone);
void draw_vectorscope(layer_t layer, vectorscope_t vectorscope);


#ifdef __cplusplus
}
#endif

#endif /* __IM_VECTORSCOPE_H__ */
