#include <stdio.h>
#include <stdlib.h>

#include <jpeg.h>
#include "kmeans.h"
#include "shapes.h"
#include "color_conversion.h"

#define DOMINANT_COLORS 5

int main(int argc, char *argv[]) {
  /**
   * Read the source image to take the tonality from
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  /**
   * Store the most dominant colors here.
   */
  vec3 dominant_colors[DOMINANT_COLORS];
  float percentage[DOMINANT_COLORS];
  vec3 variance[DOMINANT_COLORS];

  /**
   * Compute dominant colors using k-means.
   */
  kmeans(source, source.zone, DOMINANT_COLORS, dominant_colors, percentage, variance);

  printf("dominant colors\n");
  for(int i=0;i<DOMINANT_COLORS; i++) vec3_info(dominant_colors[i]);
  printf("percentage:\n");
  for(int i=0;i<DOMINANT_COLORS; i++) printf("%f\n", percentage[i]);
  printf("variance:\n");
  for(int i=0;i<DOMINANT_COLORS; i++) vec3_info(variance[i]);

  /**
   * Print and draw dominant colors.
   */
  int dx = 1024 / DOMINANT_COLORS;
  layer_t colors = layer_new_dim(DOMINANT_COLORS*dx, 512, 3, False, False);
  for(int i=0; i<DOMINANT_COLORS; i++) {
     vec3_info(dominant_colors[i]);
     rect_t shape = {i*dx, 0, i*dx+dx-1, 255};
     rect_t shape2 = {i*dx, 511-256*percentage[i], i*dx+dx-1, 511};
     rect_t shape_gauss_x = {i*dx, 256, i*dx+dx-1, 256+80};
     rect_t shape_gauss_y = {i*dx, 256+80, i*dx+dx-1, 256+160};
     rect_t shape_gauss_z = {i*dx, 256+160, i*dx+dx-1, 256+240};
     draw_filled_rect(colors, LMStoRGB(LabtoLMS(dominant_colors[i])), 1.0f, shape, blend_normal);
     //draw_filled_rect(colors, dominant_colors[i], 1.0f, shape, blend_normal);
     draw_filled_rect(colors, vec3_init(1.0f, 1.0f, 1.0f), 1.0f, shape2, blend_normal);
     draw_gauss(colors, vec3_init(1.0f, 0.0f, 0.0f), 1.0f, shape_gauss_x, variance[i].x, blend_normal);
     draw_gauss(colors, vec3_init(0.0f, 1.0f, 0.0f), 1.0f, shape_gauss_y, variance[i].y, blend_normal);
     draw_gauss(colors, vec3_init(0.0f, 0.0f, 1.0f), 1.0f, shape_gauss_z, variance[i].z, blend_normal);
  }
  write_JPEG_file("dominant-colors.jpg", colors, 90);

  return 0;
}
