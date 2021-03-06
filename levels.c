#include "levels.h"


void levels(layer_t layer, rect_t zone) {
   color_t *image = layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       float r, g, b;
       r = image[idx]/(float)COLOR_MAX;
       g = image[idx+1]/(float)COLOR_MAX;
       b = image[idx+2]/(float)COLOR_MAX;
       float Iv = 0.2f*r + 0.6f*g + 0.1f*b;
       image[idx] = image[idx+1] = image[idx+2] = COLOR_MAX/2;
       if(Iv>0.75f) { image[idx] = image[idx+1] = image[idx+2] = COLOR_MAX; }
       if(Iv<0.25f) { image[idx] = image[idx+1] = image[idx+2] = 0; }
    }
  }
}
