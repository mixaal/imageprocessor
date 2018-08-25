#include "gamma.h" 
#include <math.h>

/**
 * Adjust gamma, gamma in [0.25, 2.0]
 */
void gamma_correction(layer_t layer, float gamma, rect_t zone) {
  float gammaCorrection = 1.0f / gamma;

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
       r = (float)image[idx]/COLOR_MAX;
       g = (float)image[idx+1]/COLOR_MAX;
       b = (float)image[idx+2]/COLOR_MAX;
       float nr = COLOR_MAX * pow(r, gammaCorrection);
       float ng = COLOR_MAX * pow(g, gammaCorrection);
       float nb = COLOR_MAX * pow(b, gammaCorrection);
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
