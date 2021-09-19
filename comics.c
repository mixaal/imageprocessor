#include "comics.h"
#include "gauss.h"
#include "layer_modes.h"
#include <math.h>

/**
 * Credits:
 * https://pastebin.com/pGL6jdkm
 */

void comics_filter(layer_t layer, rect_t zone) {
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
       color_t r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       float rr = (float)r/COLOR_MAX;
       float gg = (float)g/COLOR_MAX;
       float bb = (float)b/COLOR_MAX;
       float Iv = (rr*rr+gg*gg+bb*bb) * (rr*2 + gg*1 + bb*2);
       if(Iv<0.0f) Iv=0.0f;
       if(Iv>1.0f) Iv=1.0f;
       image[idx] = image[idx+1] = image[idx+2] = (color_t) floor(COLOR_MAX*Iv);
    }
  }
}

void comics_sketch(layer_t layer, rect_t zone, _Bool emphasize) {
  color_t *image = layer.image;
  int width = layer.width;
  int height = layer.height;
  int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;

  layer_t subtract = layer_copy(layer);
  color_t *subimage = subtract.image;
#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       color_t r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       float rr = (float)r/COLOR_MAX;
       float gg = (float)g/COLOR_MAX;
       float bb = (float)b/COLOR_MAX;
       float Iv = 0.3f * rr + 0.6f * gg + 0.1f * bb;
       image[idx] = image[idx+1] = image[idx+2] = (color_t) floor(COLOR_MAX*Iv);
       subimage[idx] = subimage[idx+1] = subimage[idx+2] = (color_t) floor(COLOR_MAX*(1.0-Iv));
    }
  }
  printf("Gauss start....\n");
  gauss(subtract, 21, 0.0, zone, False);
  printf("Gauss finshed (so slow)....\n");

#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       color_t r1, g1, b1;
       color_t r2, g2, b2;
       r1 = image[idx];
       g1 = image[idx+1];
       b1 = image[idx+2];

       r2 = subimage[idx];
       g2 = subimage[idx+1];
       b2 = subimage[idx+2];
       float sr = (float)r1/COLOR_MAX;
       float sg = (float)g1/COLOR_MAX;
       float sb = (float)b1/COLOR_MAX;

       float tr = (float)r2/COLOR_MAX;
       float tg = (float)g2/COLOR_MAX;
       float tb = (float)b2/COLOR_MAX;
       vec3 a = vec3_init(sr, sg, sb);
       vec3 b = vec3_init(tr, tg, tb);
       vec3 r = blend_color_dodge(a, b, 1.0f);
       if(emphasize) {
          r.x *= r.x;
          r.x *= r.x;
       }
       image[idx] = image[idx+1] = image[idx+2] = (color_t) floor(COLOR_MAX*r.x);
    }
  }
  layer_free(subtract);
}
