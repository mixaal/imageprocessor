#include "vectorscope.h"
#include "color_conversion.h"
#include "shapes.h"
#include "xmalloc.h"

#include <stdlib.h>
#include <math.h>
#include <limits.h>


void vectorscope(image_t layer, vectorscope_t out, int *max_bin_H, int *max_bin_S, rect_t zone) {


   for(int i=0; i<HUE_BINS; i++) {
     for(int j=0; j<SATURATION_BINS; j++) {
       out[i][j] = 0;
     }
   }
   float maxSum = 0;
   color_t *image = layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       float r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       vec3 HSL = RGBtoHSL(vec3_init(r/COLOR_MAX, g/COLOR_MAX, b/COLOR_MAX));
       float H = HSL.x * 360.0f;
       float S = HSL.y * 100.0f;
       //printf("H=%f S=%f\n", H, S);  
       int bin_H = (int)H ;
       int bin_S = (int)S ;
       out[bin_H][bin_S] ++;
       if (out[bin_H][bin_S] > maxSum) {
         maxSum = out[bin_H][bin_S];
         *max_bin_H = bin_H;
         *max_bin_S = bin_S;
       }
    }
  }

   fprintf(stderr, "maxSum=%f\n", maxSum);
   for(int i=0; i<HUE_BINS; i++) {
     for(int j=0; j<SATURATION_BINS; j++) {
       out[i][j] /= (float)maxSum;
     }
   }
}

void draw_vectorscope(layer_t layer, vectorscope_t vectorscope) {
  float depth_map[layer.width][layer.height];
  for (int i=0; i<layer.width; i++){
    for (int j=0; j<layer.height; j++){
      depth_map[i][j] = 0.0f;
    }
  }
  int cx = (layer.zone.minx + layer.zone.maxx) / 2;
  int cy = (layer.zone.miny + layer.zone.maxy) / 2;

  int maxr = (cx < cy) ? cx : cy;
  maxr --;

 
   float minSum = 1.0f;
   for(int i=0; i<HUE_BINS; i++) {
     for(int j=0; j<SATURATION_BINS; j++) {
       if (vectorscope[i][j]>0 && vectorscope[i][j]<minSum) minSum = vectorscope[i][j];
     }
   }
  fprintf(stderr, "minSum=%f\n", minSum);
  float bk = log((1/0.4)/(1-minSum));
  float ak = 0.4/exp(bk*minSum);

  for (int r = maxr/10; r <= maxr; r+=maxr/10) {
    draw_circle( layer, vec3_init(1.0f, 0.0f, 0.0f), 1.0f, cx, cy, r, blend_normal);
  }
  for(int h=0; h<HUE_BINS; h++) {
    for(int s=0; s<SATURATION_BINS; s++) {
       float I = vectorscope[h][s];
       if (I > 0.0f) { 
         double a = h * M_PI / 180.0f;
         int xx = (int)(cx + maxr * s * cos(a) / 100.0f);
         int yy = (int)(cy + maxr * s * sin(a) / 100.0f);
         I = ak * exp(bk*I);
         //if (I < 0.4f) I = 0.4f;
         if  (depth_map[xx][yy]<I) {
            depth_map[xx][yy] = I;
            draw_pixel(layer, vec3_init(I, I, I), 1.0f, xx, yy, blend_normal);
         }
       }
    }
  }
}
