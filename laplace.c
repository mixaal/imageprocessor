#include "laplace.h"


static float I(color_t *image, int x, int y, int width, int color_components)
{
       int idx = y*width*color_components + x*color_components;
       color_t r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       float Iv = (0.2f*r + 0.6f*g + 0.1f*b)/COLOR_MAX;
       return Iv;
}

void laplace(layer_t layer, rect_t zone) 
{
   layer_t layer_cp = layer_copy(layer);
   color_t *image = layer.image;
   color_t *copy= layer_cp.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  for(int y=zone.miny+1; y<zone.maxy-1; y++)  {
    for(int x=zone.minx+1; x<zone.maxx-1; x++) {
       int idx = y*width*color_components + x*color_components;
       float Iv = 
       4*I(copy, x, y, width, color_components) - 
          I(copy, x+1, y, width, color_components) -
          I(copy, x-1, y, width, color_components) -
          I(copy, x, y+1, width, color_components) -
          I(copy, x, y-1, width, color_components);
       image[idx] = image[idx+1] = image[idx+2] = COLOR_MAX*Iv*0.2f;
    }
  }
 layer_free(layer_cp);
}
