#include <stdio.h>
#include <stdlib.h>

#include <jpeg.h>
#include "kmeans.h"
#include "shapes.h"
#include "color_conversion.h"

#define DOMINANT_COLORS 8

static int dominant_colors = DOMINANT_COLORS;
static void save_segmentation(const char *segmentation_filename, int width, int height, color_info_t *color_info, int *segmentation) ;

int main(int argc, char *argv[]) {
  /**
   * Read the source image to take the tonality from
   */
  if(argc>=3) dominant_colors = atoi(argv[2]);
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  /**
   * Store the most dominant colors here.
   */
  color_info_t color_info[dominant_colors];

  /**
   * Compute dominant colors using k-means.
   */
  int *segmentation = kmeans_clusters(source, source.zone, dominant_colors, color_info);
  
  save_segmentation("image-segmentation.jpg", source.width, source.height, color_info, segmentation);

  fprintf(stderr, "\n\ndominant colors\n");
  for(int i=0;i<dominant_colors; i++) vec3_info(color_info[i].color);
  fprintf(stderr, "percentage:\n");
  for(int i=0;i<dominant_colors; i++) printf("%f\n", color_info[i].percentage);
  fprintf(stderr, "variance:\n");
  for(int i=0;i<dominant_colors; i++) vec3_info(color_info[i].variance);
  fprintf(stderr, "=================\n");

  /**
   * Print and draw dominant colors.
   */
  int dx = 1024 / dominant_colors;
  layer_t colors = layer_new_dim(dominant_colors*dx, 512, 3, False, False);
  for(int i=0; i<dominant_colors; i++) {
     vec3_info(color_info[i].color);
     rect_t shape = {i*dx, 0, i*dx+dx-1, 255};
     rect_t shape2 = {i*dx, 511-256*color_info[i].percentage, i*dx+dx-1, 511};
//     rect_t shape_gauss_x = {i*dx, 256, i*dx+dx-1, 256+80};
 //    rect_t shape_gauss_y = {i*dx, 256+80, i*dx+dx-1, 256+160};
  //   rect_t shape_gauss_z = {i*dx, 256+160, i*dx+dx-1, 256+240};
     draw_filled_rect(colors, LMStoRGB(LabtoLMS(color_info[i].color)), 1.0f, shape, blend_normal);
     //draw_filled_rect(colors, dominant_colors[i], 1.0f, shape, blend_normal);
     draw_filled_rect(colors, vec3_init(1.0f, 1.0f, 1.0f), 1.0f, shape2, blend_normal);
  //   draw_gauss(colors, vec3_init(1.0f, 0.0f, 0.0f), 1.0f, shape_gauss_x, color_info[i].variance.x, blend_normal);
  //   draw_gauss(colors, vec3_init(0.0f, 1.0f, 0.0f), 1.0f, shape_gauss_y, color_info[i].variance.y, blend_normal);
  //   draw_gauss(colors, vec3_init(0.0f, 0.0f, 1.0f), 1.0f, shape_gauss_z, color_info[i].variance.z, blend_normal);
  }
  write_JPEG_file("dominant-colors.jpg", colors, 90);
  free(segmentation);

  return 0;
}


static color_info_t find_color_info(int category, color_info_t *color_info)
{
  color_info_t d;
  for(int i=0; i<dominant_colors; i++) {
     if(color_info[i].category == category) return color_info[i];
  }
  return d;
}
static void save_segmentation(const char *segmentation_filename, int width, int height, color_info_t *color_info, int *segmentation) 
{
   layer_t layer_copy = layer_new_dim(width, height, 3, False, False);
   for(int x=0; x<width; x++) for (int y=0; y<height; y++) {
       int category = segmentation[y*width + x];
       
       vec3 color;
       if (category>=0) {
           color = LMStoRGB(LabtoLMS(find_color_info(category, color_info).color));
       } else {
           color = vec3_init(0, 0, 0);
       }
       int idx = y * width * 3 + x * 3;
       layer_copy.image[idx] = COLOR_MAX * color.x;
       layer_copy.image[idx+1] = COLOR_MAX * color.y;
       layer_copy.image[idx+2] = COLOR_MAX * color.z;
   }
   for(int i=0; i<dominant_colors; i++) { 
     vec3 color = color_info[i].color;
     color.x *= 0.8f;
     color = LMStoRGB(LabtoLMS(color));
     draw_cross(
             layer_copy, 
             vec3_init(
                 color.x, 
                 color.y, 
                 color.z 
             ), 
             1.0f, width/200, 
             width * color_info[i].center.x, 
             height * color_info[i].center.y, 
             width/20, width/20
     );
   }

   write_JPEG_file(segmentation_filename, layer_copy, 90);
   layer_free(layer_copy);
}
