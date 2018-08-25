#include "gauss.h"
#include "layer.h"
#include "brush.h"
#include <stdio.h>
#include <string.h>



void brush_touch(layer_t layer, draw_mode_t canvas_select, int radius, float opacity, int x, int y, vec3 color, blend_mode_func_t blend_func) {
   color_t *image = NULL; 
   switch (canvas_select) {
   case IMAGE:
     image = layer.image;
     break;
   case MASK:
     if (layer.mask==NULL) {
       size_t nbytes = layer.width*layer.height*layer.color_components*sizeof(color_t);
       layer.mask = xmalloc(nbytes);
       memset(layer.mask, 0, nbytes);
     }
     image = layer.mask;
     break;
   default:
     return;
   }
   if(image == NULL) {
     fprintf(stderr, "rendering canvas is null, draw_mode=%d\n", canvas_select);
     return;
   }
   if ((radius&1)==0) radius++; // make it odd number
   int N2 = radius / 2;
   double **kernel = xmalloc(sizeof(double)*radius);
   for (int i=0; i<radius; i++) {
     kernel[i] = xmalloc(sizeof(double)*radius);
   }

   
   create_gauss(kernel, radius, radius/6.0, False);
   float maxval = 0;
   for (int ky = -N2; ky <= N2; ky++) {
     for (int kx = -N2; kx <= N2; kx++) {
       if (kernel[kx+N2][ky+N2] > maxval) maxval = kernel[kx+N2][ky+N2];
     }
   }


   for (int ky = -N2; ky <= N2; ky++) {
     for (int kx = -N2; kx <= N2; kx++) {
       kernel[kx+N2][ky+N2] *= 1/maxval;
     }
   }


   for (int ky = -N2; ky <= N2; ky++) {
     for (int kx = -N2; kx <= N2; kx++) {
       float g = kernel[ky+N2][kx+N2];
       int tx = x + kx;
       int ty = y + ky;
       if (tx < layer.zone.minx) tx = layer.zone.minx;
       if (ty < layer.zone.miny) ty = layer.zone.miny;
       if (tx >= layer.zone.maxx) tx = layer.zone.maxx-1;
       if (ty >= layer.zone.maxy) ty = layer.zone.maxy-1;

       int idx = layer.color_components * (tx + ty * layer.width);
       vec3 source_pixel = vec3_init(image[idx]/(float)COLOR_MAX, image[idx+1]/(float)COLOR_MAX, image[idx+2]/(float)COLOR_MAX);
       vec3 brush_pixel  = vec3_multiply(color, g); 
       vec3 blend = blend_func(brush_pixel, source_pixel, g*opacity);
       image[idx] = COLOR_MAX * blend.x;
       image[idx+1] = COLOR_MAX * blend.y;
       image[idx+2] = COLOR_MAX * blend.z;
     }
   }

}
