#include <stdio.h>
#include <stdlib.h>

#include <jpeg.h>
#include <kmeans.h>
#include "shapes.h"

int main(int argc, char *argv[]) {
  /**
   * Read the source image to take the tonality from
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  /**
   * Store the most dominant colors here.
   */
  vec3 dominant_colors[5];

  /**
   * Compute dominant colors using k-means.
   */
  kmeans(source, source.zone, 5, dominant_colors);

  /**
   * Print and draw dominant colors.
   */
  layer_t colors = layer_new_dim(5*256, 256, 3, False, False);
  for(int i=0; i<5; i++) {
     vec3_info(dominant_colors[i]);
     rect_t shape = {i*256, 0, i*256+255, 255};
     draw_filled_rect(colors, dominant_colors[i], 1.0f, shape, blend_normal);
  }
  write_JPEG_file("dominant-colors.jpg", colors, 90);

  return 0;
}
