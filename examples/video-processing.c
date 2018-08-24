#include <stdio.h>
#include <stdlib.h>

#include <filters.h>
#include <video.h>

static void filter_frame(layer_t layer, void *arg)
{
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "%s <input video file> <out video file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  process_video(argv[1], argv[2], filter_frame);
}
