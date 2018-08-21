#include <stdio.h>
#include <stdlib.h>

#include <filters.h>
#include <brush.h>
#include <flip.h>
#include <crop.h>


int main(int argc, char *argv[]) {
  layer_t sequoia_trees = read_JPEG_file(argv[1]);
  rect_t crop_zone = {0, 0, sequoia_trees.width*0.8, sequoia_trees.height};
  layer_info(sequoia_trees);
  flipX(sequoia_trees);
  crop(&sequoia_trees, crop_zone);
  adjust_color_balance(sequoia_trees, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, ALL_LEVELS, sequoia_trees.zone);
  layer_info(sequoia_trees);

  write_JPEG_file("output.jpg", sequoia_trees, 90);
}
