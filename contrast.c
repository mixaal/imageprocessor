#include "contrast.h" 


/**
 * Adjust contrast, c in [-128, 128]
 */
void contrast(layer_t layer, float c, rect_t zone) {
  float F = (259 * (c + COLOR_MAX)) / (COLOR_MAX * ( 259 - c ));

  printf("F=%f\n", F);
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
       float r, g, b;
       r = (float)image[idx] - COLOR_MID;
       g = (float)image[idx+1] - COLOR_MID;
       b = (float)image[idx+2] - COLOR_MID;
       float nr = F * r  + COLOR_MID;
       float ng = F * g  + COLOR_MID;
       float nb = F * b  + COLOR_MID;
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
