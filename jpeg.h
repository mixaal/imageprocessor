#ifndef __IM_PROC_JPEG_H__
#define __IM_PROC_JPEG_H__ 1

#include <jpeglib.h>
#include "layer.h"

image_t read_JPEG_file (char * filename);
void write_JPEG_file (char * filename, JSAMPLE *image, int image_width, int image_height, int quality);

#endif /* __IM_PROC_JPEG_H__ */
