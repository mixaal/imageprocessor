#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jpeg.h>
#include <filters.h>
#include <levels.h>


#define DEFAULT_MEDIAN_MASK_SZ 3

int main(int argc, char *argv[]) {
  int mask_size = DEFAULT_MEDIAN_MASK_SZ;
  /**
   * Read the source image
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);
  if(argc>=3) mask_size = atoi(argv[2]);

  rect_t adjust_zone = { 0, 0, source.width, source.height };
  median_filter(source, mask_size, adjust_zone);
  write_JPEG_file("median-result.jpg", source, 90);
}
