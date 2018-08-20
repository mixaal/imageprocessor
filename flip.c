#include "flip.h"

void flipX(layer_t layer)
{
  rect_t zone = layer.zone;

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
    for(int fx=0; fx<(zone.maxx-zone.minx)/2; fx++) {
       int x = zone.minx + fx;
       int dx = zone.maxx - fx;
       int src_idx = (y*width+ x)*color_components;
       int swp_idx = (y*width+dx)*color_components;
       color_t tmp = image[src_idx];
       image[src_idx] = image[swp_idx];
       image[swp_idx] = tmp;
       tmp = image[src_idx+1];
       image[src_idx+1] = image[swp_idx+1];
       image[swp_idx+1] = tmp;
       tmp = image[src_idx+2];
       image[src_idx+2] = image[swp_idx+2];
       image[swp_idx+2] = tmp;
    }
  }

}
