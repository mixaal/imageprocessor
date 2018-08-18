#ifndef __IM_BW_H__
#define __IM_BW_H__ 1

#include <stdio.h>
#include <jpeglib.h>

#include "common_types.h"
#include "layer.h"

void bw(image_t layer, rect_t zone);

#endif /* __IM_BW_H__ */
