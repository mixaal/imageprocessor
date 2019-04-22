#ifndef __IM_GAUSS_H__
#define __IM_GAUSS_H__ 1

#include <stdbool.h>
#include <stdio.h>
#include <jpeglib.h>
#include "common_types.h"
#include "xmalloc.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void gauss(image_t layer, int radius, double sigma, rect_t zone, _Bool mask);
void create_gauss(double **GKernel, int N, double sigma, _Bool normalize);


#ifdef __cplusplus
}
#endif

#endif /* __IM_GAUSS_H__ */
