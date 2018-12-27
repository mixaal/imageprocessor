#ifndef __IM_HISTOGRAM_H__
#define __IM_HISTOGRAM_H__ 1


#include "common_types.h"
#include "layer_modes.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BINS 256

typedef enum { RGB, LAB } color_space_t;

typedef struct {
  color_space_t color_space;
  int x[BINS];
  int y[BINS];
  int z[BINS];
  int samples;
} histogram_t;

void histogram_draw(layer_t layer, histogram_t histogram, _Bool);
float histogram_difference(histogram_t *h1, histogram_t *h2);

histogram_t histogram_from_layer(layer_t layer, color_space_t color_space, rect_t zone);

#ifdef __cplusplus
}
#endif





#endif


