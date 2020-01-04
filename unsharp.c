#include "unsharp.h"
#include "gauss.h"
#include "layer.h"

void unsharp(layer_t layer, int radius, float amount, rect_t zone) {
   if ((radius&1)==0) radius++; // make it odd number
 
   layer_t blur = layer_copy(layer);
   layer_t orig_copy = layer_copy(layer);
   layer_t orig_minus_blur = layer_new(layer);

   gauss(blur, radius, 0.0, zone, False); //FIXME once unsharp has mask boolean

   layer_substract(orig_minus_blur, orig_copy, blur);
   layer_multiply(orig_minus_blur, orig_minus_blur, amount);
   layer_add(layer, layer, orig_minus_blur);

   layer_free(blur);
   layer_free(orig_copy);
   layer_free(orig_minus_blur);
}
