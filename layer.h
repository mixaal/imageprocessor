#ifndef __IM_LAYER_H__
#define __IM_LAYER_H__ 1


#include <stdio.h>
#include <jpeglib.h>
#include <stdbool.h>

#ifdef __bool_true_false_are_defined
#  define _Bool bool
#endif /*__bool_true_false_are_defined */

#include "common_types.h"
#include "layer_modes.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int minx, miny, maxx, maxy;
} rect_t;

typedef enum { NORMAL, LINEAR_COLOR } blend_mode_t;

typedef enum { IMAGE, MASK } draw_mode_t;

typedef enum { sRGB, AdobeRGB } image_color_space_t ;

typedef struct {
  int rc;
  image_color_space_t color_space;
  color_t *image;
  color_t *mask;
  int width;
  int height;
  int stride;
  int color_components;
  rect_t zone;
  blend_mode_func_t blend_func;
  float opacity;
} layer_t;

layer_t layer_new_dim(int width, int height, int color_components, _Bool mask, _Bool white);
void add_layer_mask(layer_t layer, layer_t mask);
void layer_mask(layer_t *source, float intensity_threshold, float alpha_mask);
void add_layer_mask_color(layer_t layer, vec3 color);
layer_t layer_new(layer_t source);
void layer_substract(layer_t dest, layer_t from, layer_t what);
void layer_multiply(layer_t dest, layer_t source, double amount);
layer_t layer_copy(layer_t source);
void layer_free(layer_t layer);
void layer_add(layer_t dest, layer_t from, layer_t what);
layer_t layer_merge_down(int N, layer_t *layers);
void layer_info(layer_t layer);
layer_t layer_merge_two(layer_t layer1, layer_t layer2);


#ifdef __cplusplus
}
#endif

#endif /* __IM_LAYER_H__ */
