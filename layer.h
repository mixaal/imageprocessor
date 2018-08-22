#ifndef __IM_LAYER_H__
#define __IM_LAYER_H__ 1

#include <stdio.h>
#include <jpeglib.h>

#include "common_types.h"
#include "layer_modes.h"

typedef struct {
  int minx, miny, maxx, maxy;
} rect_t;

typedef enum { NORMAL, LINEAR_COLOR } blend_mode_t;

typedef enum { IMAGE, MASK } draw_mode_t;

typedef struct {
  int rc;
  color_t *image;
  color_t *mask;
  int width;
  int height;
  int stride;
  int color_components;
  rect_t zone;
  blend_mode_func_t blend_func;
  float opacity;
} image_t;

typedef image_t layer_t;

layer_t layer_new_dim(int width, int height, int color_components, _Bool mask, _Bool white);
void add_layer_mask(layer_t layer, layer_t mask);
void add_layer_mask_color(layer_t layer, vec3 color);
image_t layer_new(image_t source);
void layer_substract(image_t dest, image_t from, image_t what);
void layer_multiply(image_t dest, image_t source, double amount);
image_t layer_copy(image_t source);
void layer_free(image_t layer);
void layer_add(image_t dest, image_t from, image_t what);
layer_t layer_merge_down(int N, layer_t *layers);
void layer_info(layer_t layer);
layer_t layer_merge_two(layer_t layer1, layer_t layer2);

#endif /* __IM_LAYER_H__ */
