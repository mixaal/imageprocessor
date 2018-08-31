#ifndef __IM_BRIGHTNESS_H__
#define __IM_BRIGHTNESS_H__ 1


#include <stdio.h>
#include <jpeglib.h>
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void brightness(layer_t layer, float br, rect_t zone);

#ifdef __cplusplus
}
#endif

#endif /* __IM_BRIGHTNESS_H__ */
