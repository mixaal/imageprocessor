#ifndef __IM_DONG_2010_H__
#define __IM_DONG_2010_H__ 

#include "layer.h"

typedef enum { EMD, CLOSEST_DISTANCE} match_t;

void apply_color_dong2010(layer_t source, layer_t dest, rect_t source_zone, rect_t dest_zone, int n, match_t match, _Bool preserve_luminance);

#endif /* __IM_DONG_2010_H__ */
