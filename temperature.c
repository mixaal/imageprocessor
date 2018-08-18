#include "temperature.h" 


/**
 * Adjust temperature, temp in [-128, 128]
 */
void temperature(layer_t layer, float temp, rect_t zone) {
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
       float nr, nb;
       nr = (float)image[idx] + temp;
       nb = (float)image[idx+2] - temp;
       if (nr > 255.0f) nr = 255.0f;
       if (nb > 255.0f) nb = 255.0f;
       if (nr < 0.0f) nr = 0.0f;
       if (nb < 0.0f) nb = 0.0f;
       image[idx] = (JSAMPLE) nr;
       image[idx+2] = (JSAMPLE) nb;
    }
  }
}
