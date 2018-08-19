#include <strings.h>
#include <string.h>
#include "layer.h"
#include "xmalloc.h"
#include "common_types.h"

image_t layer_new(image_t source) {
   color_t *image_copy = xmalloc(source.width*source.height*source.color_components*sizeof(color_t));
   memset(image_copy, 0, source.width*source.height*source.color_components*sizeof(color_t));
   image_t image = source;
   image.image = image_copy;
   return image;
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
         int r = from.image[idx] + what.image[idx];
         int g = from.image[idx+1] + what.image[idx+1];
         int b = from.image[idx+2] + what.image[idx+2];

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

image_t layer_copy(image_t source) {
   color_t *image_copy = xmalloc(source.width*source.height*source.color_components*sizeof(color_t));
   bcopy(source.image, image_copy, source.width*source.height*source.color_components*sizeof(color_t));
   image_t image = source;
   image.image = image_copy;
   return image;
}

void layer_free(image_t layer) {
  if (layer.image==NULL) {
    free(layer.image);
  }
}
