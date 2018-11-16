/**
 * Adapted from this video:
 *
 *   Fantasy Scene | Blur Background: Photoshop Tutorial
 *   https://www.youtube.com/watch?v=34pTcSH06HA
 */


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
  int w = sequoia_trees.width;
  int h = sequoia_trees.height;
  rect_t blur_zone = {0, 0, 0.77f*w, 0.89f*h};
  gauss(sequoia_trees, 20, 0.0f, blur_zone);
  vec3 fantasy_orange = vec3_init(1.0f, 156.0f/255.0f, 97.0f/255.0f);
  brush_touch(sequoia_trees, IMAGE, 1000, 1.0f, 0.7f*w, 0.1f*h, fantasy_orange, blend_linear_light);
  brush_touch(sequoia_trees, IMAGE, 1000, 1.0f, 0.4f*w, 0.45f*h, fantasy_orange, blend_linear_light);
  brush_touch(sequoia_trees, IMAGE, 1000, 1.0f, 0.6f*w, 0.5f*h, fantasy_orange, blend_linear_light);
  brush_touch(sequoia_trees, IMAGE, 1000, 1.0f, 0.65f*w, 0.35f*h, fantasy_orange, blend_linear_light);
  gradient_map(sequoia_trees, vec3_init(0.26f, 0.1f, 0.47f), vec3_init(1.0f, 0.67f, 0.05f), 0.5f/*weight*/, 0.22f /*opacity*/, sequoia_trees.zone);
 
  float cyan_red[3] = { 0.0f, -0.22f, -0.03f };
  float magenta_green[3] = { 0.05f, -0.08f, -0.07f };
  float yellow_blue[3] = { 0.09f, 0.07f, 0.02f };
  adjust_color_balance(sequoia_trees, cyan_red, magenta_green, yellow_blue, false, sequoia_trees.zone);
  layer_info(sequoia_trees);

  write_JPEG_file("output.jpg", sequoia_trees, 90);
}
