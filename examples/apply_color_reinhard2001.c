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
#include <reinhard2001.h>


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
   * Read the source image to take the tonality from
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  /**
   * Read the destination image to change the tonality.
   */
  layer_t dest = read_JPEG_file(argv[2]);
  layer_info(dest);
  save_histogram("histogram-original.jpg", dest);

  /**
   * Apply tonality from source to dest.
   */
  apply_color_reinhard2001(source, dest, source.zone, dest.zone, 1.0f /*variance*/, 1.0f /*mean*/);
  write_JPEG_file("apply-color-reinhard.jpg", dest, 90);

  /**
   * Save histograms.
   */
  save_histogram("histogram-result.jpg", dest);
  save_histogram("histogram-matched.jpg", source);

}
