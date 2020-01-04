#ifndef __IM_BW_H__
#define __IM_BW_H__ 1


#include <stdio.h>
#include <jpeglib.h>

#include "common_types.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void bw(layer_t layer, rect_t zone);


#ifdef __cplusplus
}
#endif


#endif /* __IM_BW_H__ */
