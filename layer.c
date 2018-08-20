#include <strings.h>
#include <string.h>
#include "layer.h"
#include "xmalloc.h"
#include "common.h"

image_t layer_new(image_t source) {
   color_t *image_copy = xmalloc(source.width*source.height*source.color_components*sizeof(color_t));
   memset(image_copy, 0, source.width*source.height*source.color_components*sizeof(color_t));
   image_t image = source;
   image.image = image_copy;
   return image;
}

layer_t layer_new_dim(int width, int height, int color_components) {
   color_t *raster = xmalloc(width*height*color_components*sizeof(color_t));
   memset(raster, 0, width*height*color_components*sizeof(color_t));
   layer_t layer;
   layer.mask = NULL;
   layer.image = raster;
   layer.width = width;
   layer.height = height;
   layer.color_components = color_components;
   rect_t default_zone = {0, 0, width, height};
   layer.zone = default_zone;
   layer.opacity = 1.0f;
   layer.blend_mode = NORMAL;
   return layer;
}

void layer_multiply(image_t dest, image_t source, double amount) {
  int minx = min(dest.zone.minx, source.zone.minx);
  int miny = min(dest.zone.miny, source.zone.miny);
  int maxx = min(dest.zone.maxx, source.zone.maxx);
  int maxy = min(dest.zone.maxy, source.zone.maxy);


   for(int y=miny; y<maxy; y++) {
     for(int x=minx; x<maxx; x++) {
       int idx = (y*dest.width + x) * dest.color_components;
       dest.image[idx] = source.image[idx] * amount;
       dest.image[idx+1] = source.image[idx+1] * amount;
       dest.image[idx+2] = source.image[idx+2] * amount;
       if (dest.image[idx] < 0) dest.image[idx] = 0; 
       if (dest.image[idx+1] < 0) dest.image[idx+1] = 0; 
       if (dest.image[idx+2] < 0) dest.image[idx+2] = 0; 
       if (dest.image[idx] > COLOR_MAX) dest.image[idx] = COLOR_MAX; 
       if (dest.image[idx+1] > COLOR_MAX) dest.image[idx+1] = COLOR_MAX; 
       if (dest.image[idx+2] > COLOR_MAX) dest.image[idx+2] = COLOR_MAX; 
     }
   }
}

void layer_add(image_t dest, image_t from, image_t what) {
  int mix = min(dest.zone.minx, from.zone.minx);
  int miy = min(dest.zone.miny, from.zone.miny);
  int max = min(dest.zone.maxx, from.zone.maxx);
  int may = min(dest.zone.maxy, from.zone.maxy);


  int minx = min(what.zone.minx, mix);
  int miny = min(what.zone.miny, miy);
  int maxx = min(what.zone.maxx, max);
  int maxy = min(what.zone.maxy, may);
  
   for(int y=miny; y<maxy; y++) {
     for(int x=minx; x<maxx; x++) {
         int idx = (y*dest.width + x) * dest.color_components;
         float r1_coef = 1.0f;
         float g1_coef = 1.0f;
         float b1_coef = 1.0f;
         float r2_coef = 1.0f;
         float g2_coef = 1.0f;
         float b2_coef = 1.0f;
         if (from.mask != NULL) {
           r1_coef = from.mask[idx] / (float)COLOR_MAX;
           g1_coef = from.mask[idx+1] / (float) COLOR_MAX;
           b1_coef = from.mask[idx+2] / (float) COLOR_MAX;
         }
         if (what.mask != NULL) {
           r2_coef = what.mask[idx] / (float)COLOR_MAX;
           g2_coef = what.mask[idx+1] / (float) COLOR_MAX;
           b2_coef = what.mask[idx+2] / (float) COLOR_MAX;
         }
         float r = r1_coef*from.image[idx] + r2_coef*what.image[idx];
         float g = g1_coef*from.image[idx+1] + g2_coef*what.image[idx+1];
         float b = b1_coef*from.image[idx+2] + b2_coef*what.image[idx+2];

         dest.image[idx] = r;
         dest.image[idx+1] = g;
         dest.image[idx+2] = b;

         if (r>COLOR_MAX) dest.image[idx] = COLOR_MAX;
         if (g>COLOR_MAX) dest.image[idx+1] = COLOR_MAX;
         if (b>COLOR_MAX) dest.image[idx+2] = COLOR_MAX;
     }
   }

}

void layer_substract(image_t dest, image_t from, image_t what) {

  int mix = min(dest.zone.minx, from.zone.minx);
  int miy = min(dest.zone.miny, from.zone.miny);
  int max = min(dest.zone.maxx, from.zone.maxx);
  int may = min(dest.zone.maxy, from.zone.maxy);


  int minx = min(what.zone.minx, mix);
  int miny = min(what.zone.miny, miy);
  int maxx = min(what.zone.maxx, max);
  int maxy = min(what.zone.maxy, may);

   for(int y=miny; y<maxy; y++) {
     for(int x=minx; x<maxx; x++) {
       int idx = (y*dest.width + x) * dest.color_components;
         //float r1_coef = 1.0f;
         //float g1_coef = 1.0f;
         //float b1_coef = 1.0f;
         //float r2_coef = 1.0f;
         //float g2_coef = 1.0f;
         //float b2_coef = 1.0f;
         //if (from.mask != NULL) {
         //  r1_coef = from.mask[idx] / (float)COLOR_MAX;
         //  g1_coef = from.mask[idx+1] / (float) COLOR_MAX;
         //  b1_coef = from.mask[idx+2] / (float) COLOR_MAX;
         //}
         //if (what.mask != NULL) {
         // r2_coef = what.mask[idx] / (float)COLOR_MAX;
         //  g2_coef = what.mask[idx+1] / (float) COLOR_MAX;
         //  b2_coef = what.mask[idx+2] / (float) COLOR_MAX;
         //}

       if (from.image[idx] >= what.image[idx]) {
         dest.image[idx] = from.image[idx] - what.image[idx];
       } else {
         dest.image[idx] = 0;
       }
       if (from.image[idx+1] >= what.image[idx+1]) {
         dest.image[idx+1] = from.image[idx+1] - what.image[idx+1];
       } else {
         dest.image[idx+1] = 0;
       }
       if (from.image[idx+2] >= what.image[idx+2]) {
         dest.image[idx+2] = from.image[idx+2] - what.image[idx+2];
       } else {
         dest.image[idx+2] = 0;
       }
     }
   }

}

void add_layer_mask(layer_t layer, layer_t mask)
{
  layer.mask = mask.image;
}

void add_layer_mask_color(layer_t layer, vec3 color)
{
  if (layer.mask!=NULL) {
    free(layer.mask);
  }
  layer_t layer_dup = layer_copy(layer);
  for(int idx=0; idx<layer.width * layer.height * layer.color_components; idx+=layer.color_components) {
    layer_dup.image[idx] = color.x;
    layer_dup.image[idx+1] = color.y;
    layer_dup.image[idx+2] = color.z;
  }

  add_layer_mask(layer, layer_dup);
}

image_t layer_copy(image_t source) {
   color_t *image_copy = xmalloc(source.width*source.height*source.color_components*sizeof(color_t));
   bcopy(source.image, image_copy, source.width*source.height*source.color_components*sizeof(color_t));
   image_t image = source;
   image.image = image_copy;
   return image;
}

void layer_free(image_t layer) {
  if (layer.image!=NULL) {
    free(layer.image);
  }
  if (layer.mask!=NULL) {
    free(layer.mask);
  }
}

void layer_merge_down(int N, layer_t *layers) {
  if (N<=1) return;
  layer_t output = layers[0];
  for(int i=1; i<N; i++)  {
    layer_t current_layer = layers[i];
    output = layer_merge_two(current_layer, output);
  }
}

layer_t layer_merge_two(layer_t layer1, layer_t layer2) {
   float r1_coef, g1_coef, b1_coef;
   float r2_coef, g2_coef, b2_coef;
   float opacity1 = saturatef(layer1.opacity);
   float opacity2 = saturatef(layer2.opacity);
   r1_coef = g1_coef = b1_coef = opacity1;
   r2_coef = g2_coef = b2_coef = opacity2;
   layer_t output = layer_new_dim(layer1.width, layer1.height, layer1.color_components);
   output.blend_mode = NORMAL;
   output.opacity = 1.0f;
   
   for (int x=0; x<layer1.width; x++) {
     for (int y=0; y<layer1.height; y++) {
       int idx = (y*layer1.width+x) * layer1.color_components;
       if (layer1.mask!=NULL) {
         r1_coef = (float)layer1.mask[idx] / COLOR_MAX;
         g1_coef = (float)layer1.mask[idx+1] / COLOR_MAX;
         b1_coef = (float)layer1.mask[idx+2] / COLOR_MAX;
       }
       if (layer2.mask!=NULL) {
         r2_coef = opacity2 * (float)layer2.mask[idx] / COLOR_MAX;
         g2_coef = opacity2 * (float)layer2.mask[idx+1] / COLOR_MAX;
         b2_coef = opacity2 * (float)layer2.mask[idx+2] / COLOR_MAX;
       }
       float nr = opacity1 * r1_coef * layer1.image[idx] + (1 - opacity1 ) * opacity2 * r2_coef * layer2.image[idx];
       float ng = opacity1 * g1_coef * layer1.image[idx+1] + (1 - opacity1 ) * opacity2 * g2_coef * layer2.image[idx+1];
       float nb = opacity1 * b1_coef * layer1.image[idx+2] + (1 - opacity1 ) * opacity2 * b2_coef * layer2.image[idx+2];

       if (nr > COLOR_MAX) nr = COLOR_MAX;
       if (ng > COLOR_MAX) ng = COLOR_MAX;
       if (nb > COLOR_MAX) nb = COLOR_MAX;

       if (nr < 0) nr = 0.0f;
       if (ng < 0) ng = 0.0f;
       if (nb < 0) nb = 0.0f;
       output.image[idx] = (color_t) nr;
       output.image[idx+1] = (color_t) ng;
       output.image[idx+2] = (color_t) nb;
     }
   }
   return output;
}
