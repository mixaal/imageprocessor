#include <stdlib.h>
#include <stdio.h>
#include <filters.h>


int main(int argc, char *argv[]) {
  /**
   * Read the street daylight image.
   */
  layer_t comics_img = read_JPEG_file(argv[1]);
  layer_info(comics_img);

  comics(comics_img, comics_img.zone);
  write_JPEG_file("comics-out.jpg", comics_img, 90);
}


