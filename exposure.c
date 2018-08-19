#include "exposure.h" 
#include <math.h>


// And the magic of photoshop for me is that it transfers the value using a reverse gamma curve, then change them, and re-apply a gamma curve back, so the brightest pixels are less prone to be "burned

/**
 * Adjust exposure, e in [-4, 4]
 */
void exposure(layer_t layer, float e, rect_t zone) {

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
       float nr, ng, nb;
       nr = (float)image[idx] * pow(2, e);
       ng = (float)image[idx+1] * pow(2, e);
       nb = (float)image[idx+2] * pow(2, e);
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
