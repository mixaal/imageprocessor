#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jpeg.h>
#include <filters.h>
#include <levels.h>



int main(int argc, char *argv[]) {
  /**
   * Read the source image
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  rect_t adjust_zone = { 0, 0, source.width, source.height };
  //levels(source, adjust_zone);
  printf("Gaussing...\n");
  gauss(source, 3, 1.5f, adjust_zone, False);
  printf("Laplacing...\n");
  laplace(source, adjust_zone);
  write_JPEG_file("laplace-result.jpg", source, 90);
}
