#include "gamma.h" 
#include <math.h>

/**
 * Adjust gamma, gamma in [0.25, 2.0]
 */
void gamma_correction(layer_t layer, float gamma, rect_t zone) {
  float gammaCorrection = 1.0f / gamma;

  JSAMPLE *image = layer.image;
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
       r = (float)image[idx]/255.0f;
       g = (float)image[idx+1]/255.0f;
       b = (float)image[idx+2]/255.0f;
       float nr = 255.0f * pow(r, gammaCorrection);
       float ng = 255.0f * pow(g, gammaCorrection);
       float nb = 255.0f * pow(b, gammaCorrection);
       if (nr > 255.0f) nr = 255.0f;
       if (ng > 255.0f) ng = 255.0f;
       if (nb > 255.0f) nb = 255.0f;
       if (nr < 0.0f) nr = 0.0f;
       if (ng < 0.0f) ng = 0.0f;
       if (nb < 0.0f) nb = 0.0f;
       image[idx] = (JSAMPLE) nr;
       image[idx+1] = (JSAMPLE) ng;
       image[idx+2] = (JSAMPLE) nb;
    }
  }
}
