#include "common.h"
#include "color_conversion.h"
#include "vibrance.h"
#include <math.h>

// https://stackoverflow.com/questions/33966121/what-is-the-algorithm-for-vibrance-filters

static vec3 vibrance_magic(vec3 rgb, float scale);
/** 
 * Adjust saturation. [-1; 1]
 * 
 */ 
void vibrance(layer_t layer, float scale, rect_t zone) {
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
       r = (float)image[idx] ;
       g = (float)image[idx+1] ;
       b = (float)image[idx+2] ;

       vec3 v= vibrance_magic(vec3_init(r, g, b), scale);
       float nr = v.x;
       float ng = v.y;
       float nb = v.z;

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

static vec3 vibrance_magic(vec3 rgb, float scale)
{
  float x = maxf(rgb.x, rgb.y, rgb.z);
  float y = minf(rgb.x, rgb.y, rgb.z);

  float gray = to_gray(rgb);
  if (x == rgb.x && x!=y) {
    float t = fabs((rgb.y - rgb.z) / ( x - y ));
    if (t > 1.0f) t = 1.0f;
    scale = scale * (1+t) * 0.5f;
  }
  float a = (x - y) / COLOR_MAX;
  float scale1 = scale * (2 - a);
  float scale2 = 1 + scale1 * (1 - a);
  float sub = y * scale1;
  float r = rgb.x * scale2  - sub;
  float g = rgb.y * scale2  - sub;
  float b = rgb.z * scale2  - sub;

  //if (r > COLOR_MAX) r = COLOR_MAX;
  //if (g > COLOR_MAX) g = COLOR_MAX;
  //if (b > COLOR_MAX) b = COLOR_MAX;

  float gray2 = to_gray(vec3_init(r, g, b));

  r *= gray/gray2;
  g *= gray/gray2;
  b *= gray/gray2;

  float m = maxf( r, g, b );
  
  if ( m > COLOR_MAX) {
    scale = (COLOR_MAX - gray2) / (m - gray2);
    r = (r - gray2) * scale + gray2;
    g = (g - gray2) * scale + gray2;
    b = (b - gray2) * scale + gray2;
  }
  if (r > COLOR_MAX) r = COLOR_MAX;
  if (g > COLOR_MAX) g = COLOR_MAX;
  if (b > COLOR_MAX) b = COLOR_MAX;

  return vec3_init(r, g, b);
}

