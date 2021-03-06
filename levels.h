#ifndef __IM_LEVELS_H__
#define __IM_LEVELS_H__ 1


#include <stdio.h>
#include <jpeglib.h>

#include "common_types.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void levels(layer_t layer, rect_t zone);


#ifdef __cplusplus
}
#endif


#endif /* __IM_LEVELS_H__ */
