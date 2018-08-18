#include <stdio.h>
#include <stdlib.h>

#include "jpeg.h"
#include "bw.h"
#include "gauss.h"
#include "layer.h"
#include "unsharp.h"
#include "contrast.h"
#include "brightness.h"
#include "gamma.h"
#include "invert.h"
#include "temperature.h"

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
     rect_t zone = {0, 0, 1500, 1000};
     printf("BW\n");
    // bw(layer, zone);
     printf("Gauss\n");
     //gauss(layer, 20, 0.0, zone);
     //unsharp(layer, 20, 1.05, zone);
     //contrast(layer, 20, zone);
     //brightness(layer, -80, zone);
     //gamma_correction(layer, 2.0f, zone);
     //invert(layer, zone);
     temperature(layer, 20, zone);
     write_JPEG_file("output.jpg", layer.image, layer.width, layer.height, 90);
     layer_free(layer);
  }
  return EXIT_SUCCESS;
}
