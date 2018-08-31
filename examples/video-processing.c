#include <stdio.h>
#include <stdlib.h>

#include <filters.h>
#include <video.h>

static void filter_frame(layer_t layer, int frame_no, void *arg)
{
 // colorize(layer, vec3_init(0.0f, 0.0f, COLOR_MAX), 0.8f, 0.0f, ALL_LEVELS, False, layer.zone);
  //brightness(layer, -COLOR_MAX / 3, layer.zone);
 // exposure(layer, -1.9f, layer.zone);
 //auto_tone(layer, layer.zone);
 comics(layer, layer.zone);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "%s <input video file> <out video file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  process_video(argv[1], argv[2], filter_frame);
}
