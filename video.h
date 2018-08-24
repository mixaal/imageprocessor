#ifndef __IM_VIDEO_H__
#define __IM_VIDEO_H__ 

#include "layer.h"

typedef void (*filter_func_t)(layer_t layer, void *args);

void process_video(const char *filename, const char *outfilename, filter_func_t filter_func) ;

#endif /* __IM_VIDEO_H__ */
