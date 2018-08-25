#include "auto_tone.h"
#include <stdlib.h>
void auto_tone(image_t layer, rect_t zone) {
   color_t *image = layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  color_t min_r, min_g, min_b;
  color_t max_r, max_g, max_b;
  min_r = min_g = min_b = COLOR_MAX;
  max_r = max_g = max_b = 0;
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       color_t r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       if (min_r > r) min_r = r;
       if (min_g > g) min_g = g;
       if (min_b > b) min_b = b;
       if (max_r < r) max_r = r;
       if (max_g < g) max_g = g;
       if (max_b < b) max_b = b;
       if ( (min_r == min_g) && (min_g == min_b) && (min_b == 0) && (max_r == max_g) && (max_g == max_b) && (max_b == COLOR_MAX)) return;
    }
  }
 
  fprintf(stderr, "min_r=%d max_r=%d\nmin_g=%d max_g=%d\nmin_b=%d max_b=%d\n", min_r, max_r, min_g, max_g, min_b, max_b);
  float dr = max_r - min_r;
  float dg = max_g - min_g;
  float db = max_b - min_b;
  fprintf(stderr, "dr=%f dg=%f db=%f\n", dr, dg, db);
#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       color_t r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       float nr = (dr != 0) ? mix(0.0f, COLOR_MAX, (r-min_r) / dr) : r;
       float ng = (dg != 0) ? mix(0.0f, COLOR_MAX, (g-min_g) / dg) : g;
       float nb = (db != 0) ? mix(0.0f, COLOR_MAX, (b-min_b) / db) : b;

       if (nr > COLOR_MAX) nr = COLOR_MAX;
       if (ng > COLOR_MAX) ng = COLOR_MAX;
       if (nb > COLOR_MAX) nb = COLOR_MAX;

       image[idx] = (color_t) nr;
       image[idx+1] = (color_t) ng;
       image[idx+2] = (color_t) nb;
    }
  }
}
