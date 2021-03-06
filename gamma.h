#ifndef __IM_GAMMA_H__
#define __IM_GAMMA_H__ 1

#include <stdio.h>
#include <jpeglib.h>
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void gamma_correction(layer_t layer, float gamma, rect_t zone);


#ifdef __cplusplus
}
#endif

#endif /* __IM_GAMMA_H__ */
