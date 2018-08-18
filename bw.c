#include "bw.h"


void bw(image_t layer, rect_t zone) {
   JSAMPLE *image = layer.image;
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
       JSAMPLE r, g, b;
       r = image[idx];
       g = image[idx+1];
       b = image[idx+2];
       short Iv = (short)(0.2f*r + 0.6f*g + 0.1f*b);
       image[idx] = image[idx+1] = image[idx+2] = Iv;
    }
  }
}
