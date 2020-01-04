#ifndef __IM_LAPLACE_H__
#define __IM_LAPLACE_H__ 1


#include <stdio.h>
#include <jpeglib.h>

#include "common_types.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void laplace(layer_t layer, rect_t zone);


#ifdef __cplusplus
}
#endif


#endif /* __IM_LAPLACE_H__ */
