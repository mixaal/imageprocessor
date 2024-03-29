#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jpeg.h>
#include <filters.h>



int main(int argc, char *argv[]) {
  /**
   * Read the source image
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  rect_t adjust_zone = { 0, 0, source.width, source.height };
  sincity_filter(source, adjust_zone, false);
  write_JPEG_file("sincity-result.jpg", source, 90);
}
