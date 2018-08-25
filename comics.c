#include "comics.h"
#include "color_conversion.h"

void comics(image_t layer, rect_t zone) {
   color_t *image = layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  float min_Iv, max_Iv;
  min_Iv = 1.0f;
  max_Iv = 0;
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       float r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       float Iv = to_gray(vec3_init(r/COLOR_MAX, g/COLOR_MAX, b/COLOR_MAX));
       if (Iv < min_Iv ) min_Iv = Iv;
       if (Iv > max_Iv ) max_Iv = Iv;
       if (min_Iv <= 0.00001f && max_Iv == COLOR_MAX) return;
    }
  }
 
  float center_Iv = (max_Iv + min_Iv) / 2.0f;

#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       float r, g, b, nr, ng, nb;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       float Iv = to_gray(vec3_init(r/COLOR_MAX, g/COLOR_MAX, b/COLOR_MAX));
       if (Iv < center_Iv ) nr = ng = nb = 0.0f;
       else nr = ng = nb = COLOR_MAX;

       if (nr > COLOR_MAX) nr = COLOR_MAX;
       if (ng > COLOR_MAX) ng = COLOR_MAX;
       if (nb > COLOR_MAX) nb = COLOR_MAX;
       if (nr < 0 ) nr = 0;
       if (ng < 0 ) ng = 0;
       if (nb < 0 ) nb = 0;

       image[idx] = (color_t) nr;
       image[idx+1] = (color_t) ng;
       image[idx+2] = (color_t) nb;
    }
  }
}
