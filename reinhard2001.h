#ifndef __IM_REINHARD_2001_H__
#define __IM_REINHARD_2001_H__ 1

#include "common_types.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void apply_color_reinhard2001(layer_t source, layer_t dest, rect_t source_zone, rect_t dest_zone) ;

#ifdef __cplusplus
}
#endif
#endif  /* __IM_REINHARD_2001_H__ */
