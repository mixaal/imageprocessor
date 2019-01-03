#ifndef __IM_KMEANS_H__
#define __IM_KMEANS_H__ 

#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
   /**
    * Mean color intensity.
    */
   float intensity;
   /**
    * Mean color of the cluster.
    */
   vec3 color;
   /**
    * Geometric center of the cluster.
    */
   vec3 center;
   /**
    * Cluster id. 
    */
   int category;
   /**
    * Percentage of pixels in this cluster.
    */
   float percentage;
   /**
    * Number of pixels in the cluster.
    */
   size_t samples;
   /**
    * Color variance in the cluster.
    */
   vec3 variance;
} color_info_t;

int *kmeans(layer_t layer, rect_t zone, size_t n, color_info_t *result);

#ifdef __cplusplus
}
#endif

#endif /* __IM_KMEANS_H__ */
