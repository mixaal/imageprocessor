#include "contrast.h" 


/**
 * Adjust contrast, c in [-128, 128]
 */
void contrast(layer_t layer, float c, rect_t zone) {
  float F = (259 * (c + 255)) / (255 * ( 259 - c ));

  printf("F=%f\n", F);
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
       r = (float)image[idx] - 128.0f;
       g = (float)image[idx+1] - 128.0f;
       b = (float)image[idx+2] - 128.0f;
       float nr = F * r  + 128;
       float ng = F * g  + 128;
       float nb = F * b  + 128;
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
