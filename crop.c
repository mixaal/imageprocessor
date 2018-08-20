#include "crop.h"

void crop(layer_t *layer, rect_t zone) {
   int w = zone.maxx - zone.minx;
   int h = zone.maxy - zone.miny;
   rect_t default_zone = {0, 0, w, h};
   layer_t dup = layer_copy(*layer);
   for(int x = zone.minx; x<zone.maxx; x++) {
     for(int y = zone.miny; y<zone.maxy; y++) {
       int source_idx = (y * layer->width + x) * layer->color_components;
       int target_idx = ((y-zone.miny) * w + (x-zone.minx)) * layer->color_components;
       dup.image[target_idx] = layer->image[source_idx];
       dup.image[target_idx+1] = layer->image[source_idx+1];
       dup.image[target_idx+2] = layer->image[source_idx+2];
       if (layer->mask != NULL) {
         dup.mask[target_idx] = layer->mask[source_idx];
         dup.mask[target_idx+1] = layer->mask[source_idx+1];
         dup.mask[target_idx+2] = layer->mask[source_idx+2];
       }
     }
   }
   layer->image = dup.image;
   if (layer->mask != NULL) layer->mask = dup.mask;
   layer->stride = w * layer->color_components;
   layer->width = w;
   layer->height = h;
   layer->zone = default_zone;
}
