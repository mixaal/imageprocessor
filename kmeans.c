#include "kmeans.h"
#include "xmalloc.h"
#include <float.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "color_conversion.h"

#define MAX_ITERATIONS 100
#define EPSILON 0.001

static void color_info(color_info_t color_info)
{
  printf("color_info={color=[%f, %f, %f], center=[%f %f] intensity=%f, category=%d, variance=[%f %f %f]}\n",
     color_info.color.x, color_info.color.y, color_info.color.z, 
     color_info.center.x, color_info.center.y,
     color_info.intensity, color_info.category,
     color_info.variance.x, color_info.variance.y, color_info.variance.z);
}


#if 0
static float compare_colors(const color_info_t c1, const color_info_t c2) 
{
  float Iv1 = c1.intensity;
  float Iv2 = c2.intensity;
  printf("Iv1 - Iv2 = %f\n", Iv1 - Iv2);
  return Iv1 - Iv2;
}

static void sort_color_info(color_info_t *color_info, size_t n)
{
  for(int i=0; i<n; i++) {
    _Bool swapped = False;
    for (int j=0; j<n; j++) {
      //printf("swapped = %d, color_info[%d]=%p\n", swapped, i, &color_info[i]);
      if (compare_colors(color_info[i], color_info[j]) > 0.0f) {
         swapped = True;
         color_info_t temp = color_info[i];
         color_info[i] = color_info[j];
         color_info[j] = temp;
      }
    }
    if(!swapped) return;
  }
}
#endif

/**
 * Compute most dominant colors using kmeans algorithm.
 *
 * @param layer layer to compute most dominant colors
 * @param zone  zone to operate on
 * @param n     how many dominant colors to output
 * @param result resulting pre-allocated vector storing colors.
 * @return segmentation result
 */
int *kmeans(layer_t layer, rect_t zone, size_t n, color_info_t *result) {
   vec3 *new_means_sum = xmalloc(n*sizeof(vec3));
   size_t *sums = xmalloc(n*sizeof(size_t));
   printf("random vectors:\n");
   srand(time(NULL));
   color_t *image = layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;
  if (zone.maxy==0) return NULL;

   int *pixel_category = xmalloc(sizeof(int)*layer.width*layer.height);
  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  size_t zx = zone.maxx - zone.minx;
  size_t zy = zone.maxy - zone.miny;
  size_t total_pixels = zx * zy;
  size_t first_10 = 10 * total_pixels / 100;
  size_t last_10 = total_pixels - first_10;
  size_t step = (last_10 - first_10)/ n;
  size_t choose = first_10;
  
 

  /**
   * Choose N uniform samples between 10% and 90% of the image index array.
   */
  
  for(int i=0; i<n; i++) {
      sums[i] = 0;
      new_means_sum[i] = vec3_init(0.0f, 0.0f, 0.0f);
      //float sx=floor(zone.minx + (zone.maxx - zone.minx) * (float)rand() / (float)RAND_MAX);
      //float sy=floor(zone.miny + (zone.maxy - zone.miny) * (float)rand() / (float)RAND_MAX);
      //int idx = (int)sy*width*color_components + (int)sx*color_components;
      printf("choose[%d]=%ld\n", i, choose);
      int idx = choose * color_components;
      choose += step;
      
      result[i].category = i;
      result[i].color = vec3_init(
          (float)image[idx]/COLOR_MAX, (float)image[idx+1]/COLOR_MAX, (float)image[idx+2]/COLOR_MAX
      );
      result[i].intensity = to_gray(result[i].color);
      result[i].variance = vec3_init(0.0f, 0.0f, 0.0f);
      result[i].center= vec3_init(0.0f, 0.0f, 0.0f);
   }
   _Bool swapping = True;
   while(swapping) {
     swapping = False;
     for(int i=1; i<n;i++) {
       if(result[i].intensity < result[i-1].intensity) {
           swapping = True;
           color_info_t tmp = result[i-1];
           result[i-1] = result[i];
           result[i] = tmp;
       }
     }
   }
   for(int i=0; i<n; i++)  result[i].category=i;
   for(int i=0; i<n; i++)  color_info(result[i]);
   printf("======= End of Initalizaton =======\n");

  for(int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
    printf("k-means iteration: %d\n", iteration);
    for(int j=0; j<n; j++) vec3_info(result[j].color);
    printf("===================\n");
//#pragma omp parallel for
    for(int y=zone.miny; y<zone.maxy; y++)  {
      for(int x=zone.minx; x<zone.maxx; x++) {
         int idx = y*width*color_components + x*color_components;
         color_t r, g, b;
         r = image[idx];
         g = image[idx+1];
         b = image[idx+2];
       
         vec3 v = vec3_init((float)r/COLOR_MAX, (float)g/COLOR_MAX, (float)b/COLOR_MAX);
         double max_dist = FLT_MAX;
         int category = -1;
         for(int j=0; j<n; j++) {
           double d = color_distance(v, result[j].color);
           if (d < max_dist) {
             max_dist = d;
             category = j;
           }
         }
         if(category>=0) {
           new_means_sum[category] = vec3_add(new_means_sum[category], v);
           sums[category] ++;
         }
         pixel_category[y*width+x] = category;
      }
    }
    float delta = 0.0f;
    for(int j=0; j<n; j++) {
           printf("sums[%d]=%ld total_pixels=%ld\n", j, sums[j], total_pixels);
      if(sums[j] != 0 ) {
        result[j].percentage = (float)sums[j] / total_pixels;
        if(result[j].percentage < 0) {
           printf("sums[%d]=%ld total_pixels=%ld\n", j, sums[j], total_pixels);
           exit(-1);
        }
        result[j].samples = sums[j];
        new_means_sum[j].x /= sums[j];
        new_means_sum[j].y /= sums[j];
        new_means_sum[j].z /= sums[j];
        delta += vec3_dist2(new_means_sum[j], result[j].color);
        //printf("delta=%f\n", delta);
        result[j].color.x = new_means_sum[j].x;
        result[j].color.y = new_means_sum[j].y;
        result[j].color.z = new_means_sum[j].z;
      } else {
        result[j].samples = 1;
      }
      new_means_sum[j].x = 0.0f;
      new_means_sum[j].y = 0.0f;
      new_means_sum[j].z = 0.0f;
      sums[j] = 0;
    }
    if (delta < EPSILON || iteration==MAX_ITERATIONS-1) {
      float total_percentage = 0.0f;
      size_t total_sum = 0;
      for(int j=0; j<n; j++) {
           result[j].intensity = to_gray(result[j].color);
           result[j].color = LMStoLab(RGBtoLMS(result[j].color));
           result[j].variance = vec3_init(0.0f, 0.0f, 0.0f);
           result[j].center   = vec3_init(0.0f, 0.0f, 0.0f);
      }
#pragma omp parallel for
      for(int y=zone.miny; y<zone.maxy; y++)  {
        for(int x=zone.minx; x<zone.maxx; x++) {
          int idx = y*width*color_components + x*color_components;
          color_t r, g, b;
          r = image[idx];
          g = image[idx+1];
          b = image[idx+2];
       
          vec3 v = vec3_init((float)r/COLOR_MAX, (float)g/COLOR_MAX, (float)b/COLOR_MAX);
          vec3 Lab = LMStoLab(RGBtoLMS(v));
          int category = pixel_category[y*width+x]; 
          if (category>=0) {
            result[category].center   = vec3_add(result[category].center, vec3_init(
                (float)x, (float)y, 0.0f
            )); 
            result[category].variance = vec3_add(result[category].variance, vec3_init(
              pow(Lab.x - result[category].color.x, 2.0f), 
              pow(Lab.y - result[category].color.y, 2.0f), 
              pow(Lab.z - result[category].color.z, 2.0f)
            ));
          }
        }
      }

      for(int j=0; j<n; j++) {
        total_percentage += result[j].percentage;
        total_sum += result[j].samples;
        result[j].center.x /= (float) result[j].samples;
        result[j].center.y /= (float) result[j].samples;
        result[j].center.x /= (float) (zone.maxx - zone.minx);
        result[j].center.y /= (float) (zone.maxy - zone.miny);

        result[j].variance.x /= (float) result[j].samples;
        result[j].variance.y /= (float) result[j].samples;
        result[j].variance.z /= (float) result[j].samples;

        result[j].variance.x = sqrtf(result[j].variance.x);
        result[j].variance.y = sqrtf(result[j].variance.y);
        result[j].variance.z = sqrtf(result[j].variance.z);
      }
      printf("==================\n");
      printf("total perc=%f total samples=%ld\n", total_percentage, total_sum);
      printf("==================\n");
      //sort_color_info(result, n);
      for(int j=0; j<n; j++) {
           printf("result[%d]=\n", j);
           color_info(result[j]);
      }
      printf("==================\n");
      return pixel_category;
    }
  }
  printf("hopefully won't get here\n");
  //sort_color_info(result, n);
  return pixel_category;
}
