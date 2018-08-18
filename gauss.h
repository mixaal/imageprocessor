#ifndef __IM_GAUSS_H__
#define __IM_GAUSS_H__ 1

#include <stdio.h>
#include <jpeglib.h>
#include "common_types.h"
#include "xmalloc.h"
#include "layer.h"

void gauss(image_t layer, int radius, double sigma, rect_t zone);

#endif /* __IM_GAUSS_H__ */
