#include <strings.h>
#include <string.h>
#include "layer.h"
#include "xmalloc.h"
#include "common.h"
#include "color_conversion.h"

void layer_info(layer_t layer)
{
  fprintf(stderr, "layer.width=%d\nlayer.height=%d\nlayer.color_components=%d\nlayer.mask=%p\nlayer.image=%p\nlayer.opacity=%f\nlayer.blend_func=%p\nlayer.zone=[%d %d %d %d]\n", layer.width, layer.height, layer.color_components, layer.mask, layer.image, layer.opacity, layer.blend_func, layer.zone.minx, layer.zone.miny, layer.zone.maxx, layer.zone.maxy);
}

layer_t layer_new(layer_t source) {
   color_t *image_copy = xmalloc(source.width*source.height*source.color_components*sizeof(color_t));
   memset(image_copy, 0, source.width*source.height*source.color_components*sizeof(color_t));
   layer_t image = source;
   image.image = image_copy;
   return image;
}

/**
 * Create layer mask from source layer.
 * 
 * @param source              source layer to use
 * @param intensity_threshold whatever lower this intensity is black, others are white
 * @param alpha_mask          if output intensity (black/white) is below this alpha mask do not write it, useful for overlays
 */ 
void layer_mask(layer_t *source, float intensity_threshold, float alpha_mask)
{
   int width  = source -> width;
   int height = source -> height;
   int color_components = source -> color_components;
   size_t nbytes = width * height * color_components * sizeof(color_t);
   source -> mask = xmalloc(nbytes);
   for(int y=0; y<height; y++) {
     for(int x=0; x<width; x++) {
       int idx = (y*width + x) * color_components;
       vec3 c = vec3_init(
             (float)source -> image[idx]/COLOR_MAX,  
             (float)source -> image[idx+1]/COLOR_MAX,  
             (float)source -> image[idx+2]/COLOR_MAX
       );
       float Iv = to_gray(c);
       if(Iv < intensity_threshold && alpha_mask > 0) continue;
       source -> mask[idx] = (Iv >= intensity_threshold ) ? 0xff : 0x00;
       source -> mask[idx+1] = (Iv >= intensity_threshold ) ? 0xff : 0x00;
       source -> mask[idx+2] = (Iv >= intensity_threshold ) ? 0xff : 0x00;
      }
   }
}

layer_t layer_new_dim(int width, int height, int color_components, _Bool mask, _Bool white) {
   size_t nbytes = width * height * color_components * sizeof(color_t);
   color_t *raster = xmalloc(nbytes);
   memset(raster, 0, nbytes);
   layer_t layer;
   if (mask) {
     layer.mask = xmalloc(nbytes);
     memset(layer.mask, (white)  ? 0xff:0x00, nbytes);
   } else {
     layer.mask = NULL;
   }
   layer.image = raster;
   layer.width = width;
   layer.height = height;
   layer.color_components = color_components;
   rect_t default_zone = {0, 0, width, height};
   layer.zone = default_zone;
   layer.opacity = 1.0f;
   layer.blend_func = blend_normal;
   return layer;
}

void layer_multiply(layer_t dest, layer_t source, double amount) {
  int minx = min(dest.zone.minx, source.zone.minx);
  int miny = min(dest.zone.miny, source.zone.miny);
  int maxx = min(dest.zone.maxx, source.zone.maxx);
  int maxy = min(dest.zone.maxy, source.zone.maxy);


   for(int y=miny; y<maxy; y++) {
     for(int x=minx; x<maxx; x++) {
       int idx = (y*dest.width + x) * dest.color_components;
       dest.image[idx] = source.image[idx] * amount;
       dest.image[idx+1] = source.image[idx+1] * amount;
       dest.image[idx+2] = source.image[idx+2] * amount;
       if (dest.image[idx] < 0) dest.image[idx] = 0; 
       if (dest.image[idx+1] < 0) dest.image[idx+1] = 0; 
       if (dest.image[idx+2] < 0) dest.image[idx+2] = 0; 
       if (dest.image[idx] > COLOR_MAX) dest.image[idx] = COLOR_MAX; 
       if (dest.image[idx+1] > COLOR_MAX) dest.image[idx+1] = COLOR_MAX; 
       if (dest.image[idx+2] > COLOR_MAX) dest.image[idx+2] = COLOR_MAX; 
     }
   }
}

void layer_add(layer_t dest, layer_t from, layer_t what) {
  int mix = min(dest.zone.minx, from.zone.minx);
  int miy = min(dest.zone.miny, from.zone.miny);
  int max = min(dest.zone.maxx, from.zone.maxx);
  int may = min(dest.zone.maxy, from.zone.maxy);


  int minx = min(what.zone.minx, mix);
  int miny = min(what.zone.miny, miy);
  int maxx = min(what.zone.maxx, max);
  int maxy = min(what.zone.maxy, may);
  
   for(int y=miny; y<maxy; y++) {
     for(int x=minx; x<maxx; x++) {
         int idx = (y*dest.width + x) * dest.color_components;
         float r1_coef = 1.0f;
         float g1_coef = 1.0f;
         float b1_coef = 1.0f;
         float r2_coef = 1.0f;
         float g2_coef = 1.0f;
         float b2_coef = 1.0f;
         if (from.mask != NULL) {
           r1_coef = from.mask[idx] / (float)COLOR_MAX;
           g1_coef = from.mask[idx+1] / (float) COLOR_MAX;
           b1_coef = from.mask[idx+2] / (float) COLOR_MAX;
         }
         if (what.mask != NULL) {
           r2_coef = what.mask[idx] / (float)COLOR_MAX;
           g2_coef = what.mask[idx+1] / (float) COLOR_MAX;
           b2_coef = what.mask[idx+2] / (float) COLOR_MAX;
         }
         float r = r1_coef*from.image[idx] + r2_coef*what.image[idx];
         float g = g1_coef*from.image[idx+1] + g2_coef*what.image[idx+1];
         float b = b1_coef*from.image[idx+2] + b2_coef*what.image[idx+2];

         dest.image[idx] = r;
         dest.image[idx+1] = g;
         dest.image[idx+2] = b;

         if (r>COLOR_MAX) dest.image[idx] = COLOR_MAX;
         if (g>COLOR_MAX) dest.image[idx+1] = COLOR_MAX;
         if (b>COLOR_MAX) dest.image[idx+2] = COLOR_MAX;
     }
   }

}

void layer_substract(layer_t dest, layer_t from, layer_t what) {

  int mix = min(dest.zone.minx, from.zone.minx);
  int miy = min(dest.zone.miny, from.zone.miny);
  int max = min(dest.zone.maxx, from.zone.maxx);
  int may = min(dest.zone.maxy, from.zone.maxy);


  int minx = min(what.zone.minx, mix);
  int miny = min(what.zone.miny, miy);
  int maxx = min(what.zone.maxx, max);
  int maxy = min(what.zone.maxy, may);

   for(int y=miny; y<maxy; y++) {
     for(int x=minx; x<maxx; x++) {
       int idx = (y*dest.width + x) * dest.color_components;
         //float r1_coef = 1.0f;
         //float g1_coef = 1.0f;
         //float b1_coef = 1.0f;
         //float r2_coef = 1.0f;
         //float g2_coef = 1.0f;
         //float b2_coef = 1.0f;
         //if (from.mask != NULL) {
         //  r1_coef = from.mask[idx] / (float)COLOR_MAX;
         //  g1_coef = from.mask[idx+1] / (float) COLOR_MAX;
         //  b1_coef = from.mask[idx+2] / (float) COLOR_MAX;
         //}
         //if (what.mask != NULL) {
         // r2_coef = what.mask[idx] / (float)COLOR_MAX;
         //  g2_coef = what.mask[idx+1] / (float) COLOR_MAX;
         //  b2_coef = what.mask[idx+2] / (float) COLOR_MAX;
         //}

       if (from.image[idx] >= what.image[idx]) {
         dest.image[idx] = from.image[idx] - what.image[idx];
       } else {
         dest.image[idx] = 0;
       }
       if (from.image[idx+1] >= what.image[idx+1]) {
         dest.image[idx+1] = from.image[idx+1] - what.image[idx+1];
       } else {
         dest.image[idx+1] = 0;
       }
       if (from.image[idx+2] >= what.image[idx+2]) {
         dest.image[idx+2] = from.image[idx+2] - what.image[idx+2];
       } else {
         dest.image[idx+2] = 0;
       }
     }
   }

}

void add_layer_mask(layer_t layer, layer_t mask)
{
  layer.mask = mask.image;
}

void add_layer_mask_color(layer_t layer, vec3 color)
{
  if (layer.mask!=NULL) {
    free(layer.mask);
  }
  layer_t layer_dup = layer_copy(layer);
  for(int idx=0; idx<layer.width * layer.height * layer.color_components; idx+=layer.color_components) {
    layer_dup.image[idx] = color.x;
    layer_dup.image[idx+1] = color.y;
    layer_dup.image[idx+2] = color.z;
  }

  add_layer_mask(layer, layer_dup);
}

layer_t layer_copy(layer_t source) {
   layer_t image = source;
   size_t nbytes = source.width * source.height * source.color_components * sizeof(color_t);
   color_t *image_copy = xmalloc(nbytes);
   bcopy(source.image, image_copy, nbytes);
   if (source.mask!=NULL) {
     color_t *mask_copy = xmalloc(nbytes);
     bcopy(source.mask, mask_copy, nbytes);
     image.mask = mask_copy;
   } else {
     image.mask = NULL;
   }
   image.image = image_copy;
   return image;
}

void layer_free(layer_t layer) {
  if (layer.image!=NULL) {
    free(layer.image);
  }
  if (layer.mask!=NULL) {
    free(layer.mask);
  }
}

layer_t layer_merge_down(int N, layer_t *layers) {
  if (N==1) return layers[0];
  layer_t output = layers[0];
  for(int i=1; i<N; i++)  {
    layer_t current_layer = layers[i];
    output = layer_merge_two(current_layer, output);
    memcpy(layers[0].image, output.image, output.width * output.height * output.color_components * sizeof(color_t));
    layer_free(output);
    output = layers[0];
  }
  return output;
}

layer_t layer_merge_two(layer_t layer1, layer_t layer2) {
   float r1_coef, g1_coef, b1_coef;
   float r2_coef, g2_coef, b2_coef;
   float opacity1 = saturatef(layer1.opacity);
   float opacity2 = saturatef(layer2.opacity);
   printf("opacity1 = %f opacity2 = %f\n", opacity1, opacity2 );
   r1_coef = g1_coef = b1_coef = opacity1;
   r2_coef = g2_coef = b2_coef = opacity2;
   layer_t output = layer_new_dim(layer1.width, layer1.height, layer1.color_components, False, False);
   output.blend_func = blend_normal;
   output.opacity = 1.0f;
   
   for (int x=0; x<layer1.width; x++) {
     for (int y=0; y<layer1.height; y++) {
       int idx = (y*layer1.width+x) * layer1.color_components;
       if (layer1.mask!=NULL) {
         r1_coef = opacity1 * (float)layer1.mask[idx] / COLOR_MAX;
         g1_coef = opacity1 * (float)layer1.mask[idx+1] / COLOR_MAX;
         b1_coef = opacity1 * (float)layer1.mask[idx+2] / COLOR_MAX;
       }
       if (layer2.mask!=NULL) {
         r2_coef = opacity2 * (float)layer2.mask[idx] / COLOR_MAX;
         g2_coef = opacity2 * (float)layer2.mask[idx+1] / COLOR_MAX;
         b2_coef = opacity2 * (float)layer2.mask[idx+2] / COLOR_MAX;
       }
       float layer1_opacity = (r1_coef  + g1_coef  + b1_coef)  / 3.0f;
       vec3 result = layer1.blend_func(
           vec3_init(
             layer1.image[idx]/(float)COLOR_MAX, 
             layer1.image[idx+1]/(float)COLOR_MAX, 
             layer1.image[idx+2]/(float)COLOR_MAX
           ), 
           vec3_init(
             r2_coef*layer2.image[idx]/(float)COLOR_MAX,
             g2_coef*layer2.image[idx+1]/(float)COLOR_MAX,
             b2_coef*layer2.image[idx+2]/(float)COLOR_MAX
           ),
           layer1_opacity
       );
       //float nr = r1_coef * layer1.image[idx] + r2_coef * layer2.image[idx];
       //float ng = g1_coef * layer1.image[idx+1] + g2_coef * layer2.image[idx+1];
       //float nb = b1_coef * layer1.image[idx+2] + b2_coef * layer2.image[idx+2];

       float nr = COLOR_MAX * result.x;
       float ng = COLOR_MAX * result.y;
       float nb = COLOR_MAX * result.z;
       if (nr > COLOR_MAX) nr = COLOR_MAX;
       if (ng > COLOR_MAX) ng = COLOR_MAX;
       if (nb > COLOR_MAX) nb = COLOR_MAX;

       if (nr < 0) nr = 0.0f;
       if (ng < 0) ng = 0.0f;
       if (nb < 0) nb = 0.0f;
       output.image[idx] = (color_t) nr;
       output.image[idx+1] = (color_t) ng;
       output.image[idx+2] = (color_t) nb;
     }
   }
   return output;
}
