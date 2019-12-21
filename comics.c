#include "comics.h"
#include <math.h>

/**
 * Credits:
 * https://pastebin.com/pGL6jdkm
 */

void comics_filter(layer_t layer, rect_t zone) {
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
       color_t r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       float rr = (float)r/COLOR_MAX;
       float bb = (float)g/COLOR_MAX;
       float gg = (float)b/COLOR_MAX;
       float Iv = (rr*rr+gg*gg+bb*bb) * (rr*2 + gg*1 + bb*2);
       if(Iv<0.0f) Iv=0.0f;
       if(Iv>1.0f) Iv=1.0f;
       image[idx] = image[idx+1] = image[idx+2] = (color_t) floor(COLOR_MAX*Iv);
    }
  }
}
