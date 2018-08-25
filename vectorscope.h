#ifndef __IM_VECTORSCOPE_H__
#define __IM_VECTORSCOPE_H__ 1

#define HUE_BINS 360
#define SATURATION_BINS 100

#include "layer.h"

typedef float vectorscope_t[HUE_BINS][SATURATION_BINS];
void vectorscope(layer_t layer, vectorscope_t out, int *max_bin_H, int *max_bin_S, rect_t zone);
void draw_vectorscope(layer_t layer, vectorscope_t vectorscope);

#endif /* __IM_VECTORSCOPE_H__ */
