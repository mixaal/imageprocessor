#include <stdlib.h>
#include <stdio.h>
#include <filters.h>
#include <tint.h>
#include <temperature.h>
#include <exposure.h>
#include <contrast.h>
#include <color_conversion.h>
#include <unsharp.h>
#include <gradient_map.h>


int main(int argc, char *argv[]) {
  /**
   * Read the street daylight image.
   */
  layer_t source = read_JPEG_file(argv[1]);
  layer_info(source);

  temperature(source, 18*1.28f, source.zone);

  layer_t layer_dup = layer_copy(source);
  exposure(layer_dup, -0.1f, layer_dup.zone);
  contrast(layer_dup, 60.0f, layer_dup.zone);


  exposure(source, 1.3f, source.zone);
  //hdr(source, source.zone);
  //unsharp(source, 21, 1.05f, source.zone);
  contrast(source, 100.0f, source.zone); 
  write_JPEG_file("sunset.jpg", source, 90);
  layer_mask(&layer_dup, 0.05f, 0.0f);
  gradient_fill(layer_dup, vec3_init(1, 1, 1), vec3_init(0, 0, 0), 0.5f, 1.0f, vec3_init(0.65f,0.35f,0), vec3_init(-0.3f, 0.3f, 0), layer_dup.zone, 0.15f, True);

  gradient_fill(layer_dup, vec3_init(1, 1, 1), vec3_init(0, 0, 0), 0.5f, 1.0f, vec3_init(0.0f,0.5f,0), vec3_init(0.1f, 0.0f, 0), layer_dup.zone, 0.25f, True);
  
  //gauss(layer_dup, 50, 2.0, layer_dup.zone, True);

  layer_t layers[2] = { source, layer_dup };
  layer_t flattened = layer_merge_down(2, layers);
  
  write_JPEG_file("sunset2.jpg", layer_dup, 90);
  write_JPEG_file("sunset_merged.jpg", flattened, 90);
  write_JPEG_mask("sunset_mask.jpg", layer_dup, 90);

  layer_free(source);
  layer_free(layer_dup);
//  layer_free(flattened);
}


