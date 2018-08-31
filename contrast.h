#ifndef __IM_CONTRAST_H__
#define __IM_CONTRAST_H__ 1

#include <stdio.h>
#include <jpeglib.h>
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void contrast(layer_t layer, float c, rect_t zone);

#ifdef __cplusplus
}
#endif

#endif /* __IM_CONTRAST_H__ */
