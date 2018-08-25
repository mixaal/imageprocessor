#include "shapes.h"
#include <math.h>

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
