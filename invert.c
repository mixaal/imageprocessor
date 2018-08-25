#include "invert.h" 


/**
 * Invert image.
 */
void invert(layer_t layer, rect_t zone) {

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
       image[idx] = COLOR_MAX - image[idx];
       image[idx+1] = COLOR_MAX - image[idx+1];
       image[idx+2] = COLOR_MAX - image[idx+2];
    }
  }
}
