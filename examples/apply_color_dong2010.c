#include <stdio.h>
#include <stdlib.h>

#include <jpeg.h>
#include "dong2010.h"

int main(int argc, char *argv[]) {
  /**
   * Read the source image to take the tonality from
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_t dest   = read_JPEG_file(argv[2]);
  layer_info(source);

  apply_color_dong2010(source, dest, source.zone, dest.zone);
  write_JPEG_file("dong.jpg", dest, 90);

  return 0;
}
