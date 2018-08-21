#ifndef __IM_UNSHARP_H__
#define __IM_UNSHARP_H__ 1

#include "layer.h"

/** See unsharp mask, amount = 1.05, radius big enough */
void unsharp(image_t layer, int radius, float amount, rect_t zone);

#endif /* __IM_UNSHARP_H__ */
