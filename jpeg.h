#ifndef __IM_PROC_JPEG_H__
#define __IM_PROC_JPEG_H__ 1

#include <jpeglib.h>
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

image_t read_JPEG_file (const char * filename);
void write_JPEG_file (const char * filename, layer_t layer, int quality);
void write_JPEG_mask(const char * filename, layer_t layer, int quality);


#ifdef __cplusplus
}
#endif

#endif /* __IM_PROC_JPEG_H__ */
