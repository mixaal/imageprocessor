#ifndef __IM_KMEANS_H__
#define __IM_KMEANS_H__ 

#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

void kmeans(layer_t layer, rect_t zone, size_t n, vec3 *result);

#ifdef __cplusplus
}
#endif

#endif /* __IM_KMEANS_H__ */
