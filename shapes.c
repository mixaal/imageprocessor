#include "shapes.h"
#include <math.h>

void draw_gauss(layer_t layer, vec3 color, float opacity, rect_t zone, float sigma, blend_mode_func_t blend_func)
{
  float dx = (zone.maxx - zone.minx);
  float kx = dx / (6*sigma);
  float ky = (zone.maxy - zone.miny);
  for (float x = -3*sigma; x< 3*sigma; x+=sigma/100.0f) {
     float y = exp(-x*x/(2*sigma*sigma));
     draw_pixel(layer, color, opacity, zone.minx + dx/2 + kx*x, zone.maxy - ky * y, blend_func);
  }
}

void draw_circle(layer_t layer, vec3 color, float opacity, int x, int y, int r, blend_mode_func_t blend_func)
{
  for(double a=0; a<2*M_PI; a+=1.0/r) {
    int xx = (int)( 0.5 + x + r * cos(a));
    int yy = (int)( 0.5 + y + r * sin(a));
    if (xx>=layer.zone.minx && xx<layer.zone.maxx && yy>=layer.zone.miny && yy<layer.zone.maxy) {
      draw_pixel(layer, color, opacity, xx, yy, blend_func);
    }
  }
}  


vec3 get_pixel(layer_t layer, int x, int y)
{
   vec3 color = vec3_init(-1.0f, -1.0f, -1.0f);
   if (x < layer.zone.minx || x>=layer.zone.maxx) return color;
   if (y < layer.zone.miny || y>=layer.zone.maxy) return color;
   color_t *image = layer.image;
   int width = layer.width;
   int color_components = layer.color_components;
   int idx = y*width*color_components + x*color_components;

   vec3 source_pixel = vec3_init(image[idx]/(float)COLOR_MAX, image[idx+1]/(float)COLOR_MAX, image[idx+2]/(float)COLOR_MAX);
   return source_pixel;
}

void draw_cross(layer_t layer, vec3 color, float opacity, int width, int cx, int cy, int w, int h)
{
   int w2 = w >> 1;
   int h2 = h >> 1;
   int width2 = width >> 1;
   for(int x=cx-w2; x<=cx+w2; x++) for(int y=cy-width2; y<=cy+width2; y++) draw_pixel(layer, color, opacity, x, y, blend_normal);
   for(int y=cy-h2; y<=cy+h2; y++) for(int x=cx-width2; x<=cx+width2; x++) draw_pixel(layer, color, opacity, x, y, blend_normal);
}

void draw_pixel(layer_t layer, vec3 color, float opacity, int x, int y, blend_mode_func_t blend_func)
{
   if (x < layer.zone.minx || x>=layer.zone.maxx) return;
   if (y < layer.zone.miny || y>=layer.zone.maxy) return;

   color_t *image = layer.image;
   int width = layer.width;
   int color_components = layer.color_components;
   int idx = y*width*color_components + x*color_components;
   vec3 source_pixel = vec3_init(image[idx]/(float)COLOR_MAX, image[idx+1]/(float)COLOR_MAX, image[idx+2]/(float)COLOR_MAX);
   vec3 blend = blend_func(color, source_pixel, opacity);
   image[idx] = COLOR_MAX * blend.x;
   image[idx+1] = COLOR_MAX * blend.y;
   image[idx+2] = COLOR_MAX * blend.z;
}

void draw_filled_rect(layer_t layer, vec3 color, float opacity, rect_t rect, blend_mode_func_t blend_func) 
{
   color_t *image = layer.image;
   int width = layer.width;
   int color_components = layer.color_components;

   int minx = rect.minx < layer.zone.minx ? layer.zone.minx : rect.minx;
   int miny = rect.miny < layer.zone.miny ? layer.zone.miny : rect.miny;
   int maxx = rect.maxx >= layer.zone.maxx ? layer.zone.maxx-1 : rect.maxx;
   int maxy = rect.maxy >= layer.zone.maxy ? layer.zone.maxy-1 : rect.maxy;

   for(int y = miny; y<=maxy; y++) 
   for(int x = minx; x<=maxx; x++) {
       int idx;
       vec3 source_pixel, blend;
       if (rect.minx >= layer.zone.minx) {
         idx = y*width*color_components + x*color_components;
         source_pixel = vec3_init(image[idx]/(float)COLOR_MAX, image[idx+1]/(float)COLOR_MAX, image[idx+2]/(float)COLOR_MAX);
         blend = blend_func(color, source_pixel, opacity);
         image[idx] = COLOR_MAX * blend.x;
         image[idx+1] = COLOR_MAX * blend.y;
         image[idx+2] = COLOR_MAX * blend.z;
       }
   }
 
}


void draw_rect(layer_t layer, vec3 color, float opacity, rect_t rect, blend_mode_func_t blend_func)
{
   color_t *image = layer.image;
   int width = layer.width;
   int color_components = layer.color_components;

   int minx = rect.minx < layer.zone.minx ? layer.zone.minx : rect.minx;
   int miny = rect.miny < layer.zone.miny ? layer.zone.miny : rect.miny;
   int maxx = rect.maxx >= layer.zone.maxx ? layer.zone.maxx-1 : rect.maxx;
   int maxy = rect.maxy >= layer.zone.maxy ? layer.zone.maxy-1 : rect.maxy;

   for(int y = miny; y<=maxy; y++) {
       int idx;
       vec3 source_pixel, blend;
       if (rect.minx >= layer.zone.minx) {
         idx = y*width*color_components + minx*color_components;
         source_pixel = vec3_init(image[idx]/(float)COLOR_MAX, image[idx+1]/(float)COLOR_MAX, image[idx+2]/(float)COLOR_MAX);
         blend = blend_func(color, source_pixel, opacity);
         image[idx] = COLOR_MAX * blend.x;
         image[idx+1] = COLOR_MAX * blend.y;
         image[idx+2] = COLOR_MAX * blend.z;
       }
       if (rect.maxx < layer.zone.maxx) {
         idx = y*width*color_components + maxx*color_components;
         source_pixel = vec3_init(image[idx]/(float)COLOR_MAX, image[idx+1]/(float)COLOR_MAX, image[idx+2]/(float)COLOR_MAX);
         blend = blend_func(color, source_pixel, opacity);
         image[idx] = COLOR_MAX * blend.x;
         image[idx+1] = COLOR_MAX * blend.y;
         image[idx+2] = COLOR_MAX * blend.z;
       }
   }
   for(int x = minx; x<=maxx; x++) {
       int idx;
       vec3 source_pixel, blend;
       if (rect.miny >= layer.zone.miny ){
         idx = miny*width*color_components + x*color_components;
         source_pixel = vec3_init(image[idx]/(float)COLOR_MAX, image[idx+1]/(float)COLOR_MAX, image[idx+2]/(float)COLOR_MAX);
         blend = blend_func(color, source_pixel, opacity);
         image[idx] = COLOR_MAX * blend.x;
         image[idx+1] = COLOR_MAX * blend.y;
         image[idx+2] = COLOR_MAX * blend.z;
       }
       
       if (rect.maxy < layer.zone.maxy ){
         idx = maxy*width*color_components + x*color_components;
         source_pixel = vec3_init(image[idx]/(float)COLOR_MAX, image[idx+1]/(float)COLOR_MAX, image[idx+2]/(float)COLOR_MAX);
         blend = blend_func(color, source_pixel, opacity);
         image[idx] = COLOR_MAX * blend.x;
         image[idx+1] = COLOR_MAX * blend.y;
         image[idx+2] = COLOR_MAX * blend.z;
       }
   }
 
}
