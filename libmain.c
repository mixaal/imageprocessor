#include <stdio.h>
#include <stdlib.h>

#include "filters.h"
#include "libmain.h"
#include "histogram.h"
#include "jpeg.h"

#define LIVEVIEW_FILE "lifeview.jpg"
#define RGB_HISTOGRAM_FILE "rgbhistogram.jpg"
#define LEVELS_FILE "levels.jpg"

static void save_histogram(const char *filename, layer_t layer)
{
  layer_t hist_layer = layer_new_dim(512, 300, 3, false, false);
  histogram_t h = histogram_from_layer(layer, RGB, layer.zone);
  histogram_draw(hist_layer, h, true);
  write_JPEG_file(filename, hist_layer, 90);
  layer_free(hist_layer);
}


void get_liveview_histogram(void)
{
  layer_t life_view = read_JPEG_file( LIVEVIEW_FILE );
  save_histogram(RGB_HISTOGRAM_FILE, life_view);
}

void get_liveview_levels(int show_areas_out_of_dynamic_range)
{
  layer_t source = read_JPEG_file(LIVEVIEW_FILE);

  rect_t adjust_zone = { 0, 0, source.width, source.height };
  levels(source, adjust_zone);
  write_JPEG_file(LEVELS_FILE, source, 90);
}
