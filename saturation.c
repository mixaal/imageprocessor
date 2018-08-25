#include "common.h"
#include "color_conversion.h"
#include "saturation.h"

/** 
 * Adjust saturation. [-1; 1]
 * 
 */ 
void saturation(layer_t layer, float amount, rect_t zone) {
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
       r = (float)image[idx] / COLOR_MAX;
       g = (float)image[idx+1] / COLOR_MAX;
       b = (float)image[idx+2] / COLOR_MAX;

       vec3 HSL = RGBtoHSL(vec3_init(r, g, b));
       HSL.y += amount;
       HSL.y = saturatef(HSL.y);
       

       vec3 newRGB = HSLtoRGB(HSL);
       
       float nr = COLOR_MAX * newRGB.x;
       float ng = COLOR_MAX * newRGB.y;
       float nb = COLOR_MAX * newRGB.z;
       if (nr > COLOR_MAX) nr = COLOR_MAX;
       if (ng > COLOR_MAX) ng = COLOR_MAX;
       if (nb > COLOR_MAX) nb = COLOR_MAX;
       if (nr < 0.0f) nr = 0.0f;
       if (ng < 0.0f) ng = 0.0f;
       if (nb < 0.0f) nb = 0.0f;
       image[idx] = (color_t) nr;
       image[idx+1] = (color_t) ng;
       image[idx+2] = (color_t) nb;
    }
  }

}

