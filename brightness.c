#include "brightness.h" 


/**
 * Adjust brightness, br in [-128, 128]
 */
void brightness(layer_t layer, float br, rect_t zone) {

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
       float nr, ng, nb;
       nr = (float)image[idx] + br;
       ng = (float)image[idx+1] + br;
       nb = (float)image[idx+2] + br;
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
