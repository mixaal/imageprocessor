#include "kmeans.h"
#include "xmalloc.h"
#include <float.h>

#define MAX_ITERATIONS 100
#define EPSILON 0.001

/**
 * Compute most dominant colors using kmeans algorithm.
 *
 * @param layer layer to compute most dominant colors
 * @param zone  zone to operate on
 * @param n     how many dominant colors to output
 * @param result resulting pre-allocated vector storing colors.
 */
void kmeans(layer_t layer, rect_t zone, size_t n, vec3 *result) {
   vec3 *new_means_sum = xmalloc(n*sizeof(vec3));
   size_t *sums = xmalloc(n*sizeof(size_t));

   for(int i=0; i<n; i++) {
      sums[i] = 0;
      new_means_sum[i] = vec3_init(0.0f, 0.0f, 0.0f);
      result[i] = vec3_init(
         (float)rand() / (float)RAND_MAX,
         (float)rand() / (float)RAND_MAX,
         (float)rand() / (float)RAND_MAX
      );
   }
   color_t *image = layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  for(int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
    printf("k-means iteration: %d\n", iteration);
#pragma omp parallel for
    for(int y=zone.miny; y<zone.maxy; y++)  {
      for(int x=zone.minx; x<zone.maxx; x++) {
         int idx = y*width*color_components + x*color_components;
         color_t r, g, b;
         r = image[idx];
         g = image[idx+1];
         b = image[idx+2];
       
         vec3 v = vec3_init((float)r/COLOR_MAX, (float)g/COLOR_MAX, (float)b/COLOR_MAX);
         float max_dist = FLT_MAX;
         int category = 0;
         for(int j=0; j<n; j++) {
           float d = vec3_dist(v, result[j]);
           if (d < max_dist) {
             max_dist = d;
             category = j;
           }
         }
         new_means_sum[category] = vec3_add(new_means_sum[category], v);
         sums[category] ++;
      }
    }
    float delta = 0.0f;
    for(int j=0; j<n; j++) {
      printf("sums[%d]=%ld\n", j, sums[j]);
      if(sums[j] != 0 ) {
        new_means_sum[j].x /= sums[j];
        new_means_sum[j].y /= sums[j];
        new_means_sum[j].z /= sums[j];
        delta += vec3_dist(new_means_sum[j], result[j]);
        printf("delta=%f\n", delta);
        result[j].x = new_means_sum[j].x;
        result[j].y = new_means_sum[j].y;
        result[j].z = new_means_sum[j].z;
      }
      new_means_sum[j].x = 0.0f;
      new_means_sum[j].y = 0.0f;
      new_means_sum[j].z = 0.0f;
      sums[j] = 0;
    }
    if (delta < EPSILON) return;
  }
}
