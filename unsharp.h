#ifndef __IM_UNSHARP_H__
#define __IM_UNSHARP_H__ 1

#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

/** See unsharp mask, amount = 1.05, radius big enough */
void unsharp(image_t layer, int radius, float amount, rect_t zone);

#ifdef __cplusplus
}
#endif

#endif /* __IM_UNSHARP_H__ */
