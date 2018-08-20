#include <stdio.h>
#include <stdlib.h>

#include "filters.h"
#include "brush.h"

int main(int argc, char *argv[]) 
{
  if (argc < 3) {
     fprintf(stderr, "Usage:\n   %s <script_file> <image1> [image2 .... ]\n", argv[0]);
     exit(EXIT_FAILURE);
  }
  for (int i=2; i<argc; i++) {
     fprintf(stderr, "Processing %s\n", argv[i]); 
     image_t layer = read_JPEG_file(argv[i]);
     printf("w=%d h=%d stride=%d\n", layer.width, layer.height, layer.stride);
     if (!layer.rc) continue;
     rect_t zone = {0, 0, layer.width/2, layer.height};
     printf("BW\n");
    // bw(layer, zone);
     printf("Gauss\n");
     //gauss(layer, 20, 0.0, zone);
     //unsharp(layer, 20, 1.05, zone);
     //contrast(layer, 20, zone);
     //brightness(layer, -80, zone);
     //gamma_correction(layer, 2.0f, zone);
     //invert(layer, zone);
     //temperature(layer, 20, zone);
     //tint(layer, 20, zone);
     //exposure(layer, 1.1f, zone);
     // kelvin_temperature(layer,13000.0f, 1.0f, zone);
     adjust_color_saturation(layer, 0.3f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, zone);
     //saturation(layer, 1.0f, zone);
     //vibrance(layer, 0.5f, zone);
     //colorize(layer, vec3_init(COLOR_MAX, 0, 0), 0.5, 0.0, zone);
     layer_t brush_layer = layer_new_dim(layer.width, layer.height, 3, True, False);
     printf("brush_layer.image=%p brush_layer.mask=%p\n", brush_layer.image, brush_layer.mask);
     brush_touch(brush_layer, IMAGE, 100, 1.0f, 100, 100, vec3_init(1.0f, 1.0f, 1.0f));
     printf("brush_layer.image=%p brush_layer.mask=%p\n", brush_layer.image, brush_layer.mask);
     brush_touch(brush_layer, IMAGE, 500, 1.0f, 500, 500, vec3_init(1.0f, 1.0f, 0.0f));
     printf("brush_layer.image=%p brush_layer.mask=%p\n", brush_layer.image, brush_layer.mask);
     brush_touch(brush_layer, MASK, 100, 1.0f, 100, 100, vec3_init(1.0f, 1.0f, 1.0f));
     printf("brush_layer.image=%p brush_layer.mask=%p\n", brush_layer.image, brush_layer.mask);
     brush_touch(brush_layer, MASK, 500, 1.0f, 500, 500, vec3_init(1.0f, 1.0f, 1.0f));
     printf("brush_layer.image=%p brush_layer.mask=%p\n", brush_layer.image, brush_layer.mask);
     layer_t layers[] = { layer, brush_layer };
     brush_layer.opacity = 0.5f;
     layer_t output = layer_merge_down(2, layers);
     write_JPEG_file("output.jpg", output.image, layer.width, layer.height, 90);
     layer_free(layer);
  }
  return EXIT_SUCCESS;
}
