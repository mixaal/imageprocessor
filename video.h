#ifndef __IM_VIDEO_H__
#define __IM_VIDEO_H__ 

#include "layer.h"

typedef void (*filter_func_t)(layer_t layer, int frame_no, void *args);

#ifdef __cplusplus
extern "C" {
#endif

void process_video(const char *filename, const char *outfilename, filter_func_t filter_func) ;


#ifdef __cplusplus
}
#endif

#endif /* __IM_VIDEO_H__ */
