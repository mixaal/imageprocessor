#include <stdio.h>
#include <stdlib.h>

#include <jpeg.h>
#include <color_balance.h>
#include <filters.h>
#include <brush.h>
#include <flip.h>
#include <crop.h>
#include <apply_color.h>
#include <histogram.h>
#include <auto_tone.h>


static void save_histogram(const char *filename, layer_t layer)
{

  layer_t hist_layer = layer_new_dim(512, 300, 3, false, false);
  histogram_t h = histogram_from_layer(layer, RGB, layer.zone);
  histogram_draw(hist_layer, h, true);
  write_JPEG_file(filename, hist_layer, 90);
  layer_free(hist_layer);
}

int main(int argc, char *argv[]) {
  /**
   * Read the source image
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  layer_t scaled_dest = read_JPEG_file(argv[2]);

  layer_t dest = read_JPEG_file(argv[3]);
  layer_t dest_copy = layer_copy(dest);
  layer_info(dest);
  save_histogram("histogram-original.jpg", dest);

  apply_color_params_t p = apply_color(source, source.zone, scaled_dest, 8, false);
  
  adjust_color_balance(dest_copy, p.cyan_red_coef, p.magenta_green_coef, p.yellow_blue_coef, false, dest_copy.zone);
  dest.opacity = 0.85f;
  dest_copy.opacity =  1.0f - dest.opacity;
  dest_copy.blend_func = blend_linear_light;
  layer_t layers[2] =  { dest, dest_copy };
  layer_t output = layer_merge_down( 2, layers );

  layer_info(output);
  write_JPEG_file("apply-color-result.jpg", output, 90);

  save_histogram("histogram-result.jpg", dest);
  save_histogram("histogram-matched.jpg", source);

}
