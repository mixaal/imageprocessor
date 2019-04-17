#include <stdio.h>
#include <stdlib.h>

#include <jpeg.h>
#include "dong2010.h"
#include "common.h"
#include "color_conversion.h"
#include "contrast.h" 
#include "saturation.h"

int main(int argc, char *argv[]) {
  /**
   * Read the source image to take the tonality from
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_t dest   = read_JPEG_file(argv[2]);
  int colors = 32;
  int contrast_value = 0;
  float saturation_value = 0.0f;

  if(argc>3) {
    colors = atoi(argv[3]);
    fprintf(stderr, "Custom number for colors segmentation: %d\n", colors);
  }
  if(argc>4) {
    contrast_value = atoi(argv[4]);
    fprintf(stderr, "Custom contrast adjustment: %d\n", contrast_value);
  }
  if(argc>5) {
    saturation_value = atof(argv[5]);
    fprintf(stderr, "Custom saturation adjustment: %.3f\n", saturation_value);
  }
  layer_info(source);

  apply_color_dong2010(source, dest, source.zone, dest.zone, colors, CLOSEST_DISTANCE, True);
  if(saturation_value!=0.0f) saturation(dest, saturation_value, dest.zone);
  if(contrast_value!=0) contrast(dest, (float)contrast_value, dest.zone);
  write_JPEG_file("dong.jpg", dest, 90);

  return 0;
}
