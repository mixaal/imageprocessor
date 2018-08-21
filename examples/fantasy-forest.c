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
  
  gradient_map(sequoia_trees, vec3_init(0.26f, 0.1f, 0.47f), vec3_init(1.0f, 0.67f, 0.05f), 0.5f/*weight*/, 0.22f /*opacity*/, sequoia_trees.zone);
 
  //adjust_color_balance(sequoia_trees, 0.0f, 0.05f, 0.09f, 0.6f, 0.0f, SHADOWS, sequoia_trees.zone);
  //adjust_color_balance(sequoia_trees, -0.22f, -0.08f, 0.07f, 0.6f, 0.0f, MIDTONES, sequoia_trees.zone);
  //adjust_color_balance(sequoia_trees, -0.03f, -0.07f, 0.02f, 0.6f, 0.0f, HIGHLIGHTS, sequoia_trees.zone);
  layer_info(sequoia_trees);

  write_JPEG_file("output.jpg", sequoia_trees, 90);
}
