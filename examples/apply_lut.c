#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jpeg.h>
#include <color_balance.h>
#include <filters.h>
#include <brush.h>
#include <flip.h>
#include <crop.h>
#include <histogram.h>
#include <lut.h>


static void save_histogram(const char *filename, layer_t layer)
{

  layer_t hist_layer = layer_new_dim(512, 300, 3, false, false);
  histogram_t h = histogram_from_layer(layer, RGB, layer.zone);
  histogram_draw(hist_layer, h, true);
  write_JPEG_file(filename, hist_layer, 90);
  layer_free(hist_layer);
}

int main(int argc, char *argv[]) {
  /**
   * Read the source image
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  lut_t lut;
  _Bool ok = read_lut(argv[2], &lut);
  if(!ok) {
    fprintf(stderr, "Can't read lut from %s\n", argv[2]);
    exit(EXIT_FAILURE);
  }
  _Bool fast = 0;
  if(argc>3 && !strcmp(argv[3], "fast=true")) fast=1;
  
  save_histogram("histogram-before-lut.jpg", source);
  if(fast && (lut.lut_size==64 || lut.lut_size==32)) fprintf(stderr, "Using fast lookup table\n"); else fprintf(stderr, "Using slow trilinear interpolation\n");
  rect_t adjust_zone = { source.width >> 1, 0, source.width, source.height };
  lut_translate(source, lut, adjust_zone, fast);
  write_JPEG_file("apply-lut-result.jpg", source, 90);
  save_histogram("histogram-after-lut.jpg", source);

}
