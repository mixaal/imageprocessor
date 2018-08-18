#ifndef __IM_CONTRAST_H__
#define __IM_CONTRAST_H__ 1

#include <stdio.h>
#include <jpeglib.h>
#include "layer.h"

void contrast(layer_t layer, float c, rect_t zone);

#endif /* __IM_CONTRAST_H__ */
