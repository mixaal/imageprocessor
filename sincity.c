#include "sincity.h"
#include <math.h>

static float smoothstep(float a, float b, float x);
static float lerp(float a, float b, float w);
static float __saturatef(float x);

/**
 * Credits:
 *   https://pastebin.com/zVxsZBVG
 */

void sincity_filter(layer_t layer, rect_t zone, _Bool colorized) {
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
       r = image[idx] / (float) COLOR_MAX;
       g = image[idx+1] / (float) COLOR_MAX;
       b = image[idx+2] / (float) COLOR_MAX;
       r = powf(r, 0.45f);
       g = powf(g, 0.45f);
       b = powf(b, 0.45f);
       float Iv = r*0.3f + g*0.3f + b*0.3f;
       float weight = smoothstep(.1f, 0.25f, r - Iv);
       Iv = powf(Iv * 1.1f, 2.0f);
       if(colorized) {
         r = lerp(Iv, r*1.1f, weight);
	 g = lerp(Iv, g*0.5f, weight);
	 b = lerp(Iv, b*0.5f, weight);
         r = powf(r, 2.2f);
         g = powf(g, 2.2f);
         b = powf(b, 2.2f);
         color_t or = COLOR_MAX * r;
         color_t og = COLOR_MAX * g;
         color_t ob = COLOR_MAX * b;
         if(or<0) or=0;
         if(og<0) og=0;
         if(ob<0) ob=0;
         if(or>COLOR_MAX) or=COLOR_MAX;
         if(og>COLOR_MAX) og=COLOR_MAX;
         if(ob>COLOR_MAX) ob=COLOR_MAX;
         image[idx] = or;
         image[idx+1] = og;
         image[idx+2] = ob;
       } else {
         Iv = lerp(Iv, r*1.1f +  b*0.5f + g*0.5f, weight);
         Iv = powf(Iv, 2.2f);
         if(Iv<0.0f) Iv=0.0f;
         if(Iv>1.0f) Iv=1.0f;
         image[idx] = image[idx+1] = image[idx+2] = (color_t) floor(COLOR_MAX*Iv);
       }
     }
  }
}

static float lerp(float a, float b, float w)
{
  return a + w*(b-a);
}

static float __saturatef(float x)
{
  return max(0, min(1, x));
}

static float smoothstep(float a, float b, float x)
{
    float t = __saturatef((x - a)/(b - a));
    return t*t*(3.0 - (2.0*t));
}


