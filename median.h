#ifndef __IM_MEDIAN_H__
#define __IM_MEDIAN_H__ 1


#include <stdio.h>
#include <jpeglib.h>

#include "common_types.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void median_filter(layer_t layer, rect_t zone);


#ifdef __cplusplus
}
#endif


#endif /* __IM_MEDIAN_H__ */
