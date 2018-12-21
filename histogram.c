#include "histogram.h"
#include "color_conversion.h"
#include "shapes.h"
#include <string.h>
#include <math.h>

static histogram_t histogram_init() {
  histogram_t h;
  memset(&h, 0, sizeof(h));
  return h;
}


static void histogram_sample(histogram_t *h, int r, int g, int b)
{
   h->samples ++;
   h->x[r] ++;
   h->y[g] ++;
   h->z[b] ++;
}

static void hdraw(layer_t layer, vec3 color, int x, int y1, int y2)
{
   int sy = y1;
   int ey = y2;
 
   if (y1>y2) { 
     sy = y2; 
     ey = y1;
   }

     for(int y=sy; y<ey; y++) {
         float opacity = 1.0f;
         vec3 c = get_pixel(layer, x, y);
         if (c.x != 0.0f || c.y != 0.0f || c.z != 0.0f) opacity = 0.5f;
         draw_pixel(layer, color, opacity, x, y, blend_normal);
     }
}

void histogram_draw(layer_t layer, histogram_t h, _Bool log_scale)
{
   int max_samples = 1;
   int step = layer.width / BINS;
   
   for(int i = 0 ; i<BINS; i++ ) {
      if (h.z[i] > max_samples) max_samples = h.z[i];
      if (h.y[i] > max_samples) max_samples = h.y[i];
      if (h.x[i] > max_samples) max_samples = h.x[i];
   }
   double maxY = max_samples;
   if(log_scale) {
      maxY = log(max_samples);
   }
   int hy = layer.height/3;
   for(int i = 0 ; i<BINS; i++ ) {
      int b_cnt = h.z[i];
      int r_cnt = h.x[i];
      int g_cnt = h.y[i];
      
      int draw_b = hy*b_cnt / max_samples;
      int draw_g = hy*g_cnt / max_samples;
      int draw_r = hy*r_cnt / max_samples;
      if(log_scale) {
          if(draw_b!=0) draw_b = (int)(hy * log(b_cnt) / maxY);
          if(draw_g!=0) draw_g = (int)(hy * log(g_cnt) / maxY);
          if(draw_r!=0) draw_r = (int)(hy * log(r_cnt) / maxY);
      }
      for(int j=0; j<step; j++) {
        hdraw(layer, vec3_init(0.0f, 0.0f, 1.0f), i*step+j, layer.height-2*hy, layer.height-2*hy-draw_b);
        hdraw(layer, vec3_init(1.0f, 0.0f, 0.0f), i*step+j, layer.height-hy, layer.height-hy-draw_r);
        hdraw(layer, vec3_init(0.0f, 1.0f, 0.0f), i*step+j, layer.height, layer.height-draw_g);
      }
   }
}

histogram_t histogram_from_layer(layer_t layer, color_space_t color_space, rect_t zone)
{
   color_t *image = layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
   histogram_t h = histogram_init();
   if (zone.maxy==0) return h;

   if (zone.minx<0) zone.minx=0;
   if (zone.miny<0) zone.miny=0;
   if (zone.maxx>=width) zone.maxx=width;
   if (zone.maxy>=height) zone.maxy=height;


  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       color_t r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       //vec3 v = vec3_init((float)r/COLOR_MAX, (float)g/COLOR_MAX, (float)b/COLOR_MAX);
       
       switch(color_space) {
           case Lab:
               //histogram_sample(&h, RGB2Lab(v));
               break;
           case RGB:
           default: 
               //histogram_sample(&h, v);
               histogram_sample(&h, r, g, b);
       }
    }
  }

  return h;
}

float histogram_difference(histogram_t *h1, histogram_t *h2)
{
   float se = 0.0f;
   for (int i=1; i<BINS; i++) {
     float dx = h1->x[i] - h2->x[i];
     float dy = h1->y[i] - h2->y[i];
     float dz = h1->z[i] - h2->z[i];
     se += dx*dx + dy*dy + dz*dz;
   }
   return se;
}
