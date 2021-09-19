#ifndef __IM_COMICS_H__
#define __IM_COMICS_H__ 1


#include <stdio.h>
#include <jpeglib.h>

#include "common_types.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void comics_filter(layer_t layer, rect_t zone);
void comics_sketch(layer_t layer, rect_t zone);


#ifdef __cplusplus
}
#endif


#endif /* __IM_COMICS_H__ */
