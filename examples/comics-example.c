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
// rect_t invert_zone = { 0, 0, source.width*0.31f, source.height*0.87f };
//  comics_filter(source, adjust_zone);
  comics_sketch(source, adjust_zone, False);
//  invert(source, invert_zone);
  write_JPEG_file("comics-result.jpg", source, 100);
}
