#ifndef __IM_LAYER_H__
#define __IM_LAYER_H__ 1

#include <stdio.h>
#include <jpeglib.h>

typedef struct {
  int minx, miny, maxx, maxy;
} rect_t;

typedef struct {
  int rc;
  JSAMPLE *image;
  int width;
  int height;
  int stride;
  int color_components;
  rect_t zone;
} image_t;

typedef image_t layer_t;

image_t layer_new(image_t source);
void layer_substract(image_t dest, image_t from, image_t what);
void layer_multiply(image_t dest, image_t source, double amount);
image_t layer_copy(image_t source);
void layer_free(image_t layer);
void layer_add(image_t dest, image_t from, image_t what);

#endif /* __IM_LAYER_H__ */
