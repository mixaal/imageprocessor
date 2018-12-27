/**
 * E. Reinhard, M. Ashikhmin, B. Gooch, P. Shirley, "Color transfer between images", 
 * IEEE Comput. Graph. Appl., vol. 21, no. 5, pp. 34-41, 2001.
 */ 

#include "color_conversion.h"
#include "reinhard2001.h"
#include <math.h>

struct mean_and_variance {
  float L_m, L_s, a_m, a_s, b_m, b_s;
};

static void apply_reinhard2001(layer_t source, layer_t dest, rect_t source_zone, rect_t dest_zone, struct mean_and_variance src_stat, struct mean_and_variance dst_stat, float variance_coef, float mean_coef);
static struct mean_and_variance compute_mean_and_variance(layer_t layer, rect_t zone);
static void print_stat(const char *msg, struct mean_and_variance r);


/**
 * Apply color transform from source image to destination image. 
 * 
 * @param source - source image to take
 * @param dest - destination image to tone. 
 * @param source_zone - compute the statistics from the source region
 * @param dest_zone - zone in destination image to influence
 *
 */ 
void apply_color_reinhard2001(layer_t source, layer_t dest, rect_t source_zone, rect_t dest_zone, float variance_coef, float mean_coef) {
   struct mean_and_variance src_stat = compute_mean_and_variance(source, source_zone);
   print_stat("source     :", src_stat);
   struct mean_and_variance dst_stat = compute_mean_and_variance(dest, dest_zone);
   print_stat("destination:", dst_stat);
   apply_reinhard2001(source, dest, source_zone, dest_zone, src_stat, dst_stat, variance_coef, mean_coef);
   struct mean_and_variance result_stat = compute_mean_and_variance(dest, dest_zone);
   print_stat("result     :", result_stat);
}

static void print_stat(const char *msg, struct mean_and_variance r)
{
  printf("%s: Lm=[%f, %f, %f]  Ls=[%f, %f %f]\n", msg, r.L_m, r.a_m, r.b_m, r.L_s, r.a_s, r.b_s);
}

static struct mean_and_variance compute_mean_and_variance(layer_t layer, rect_t zone)
{
    color_t *image = layer.image;
   int width = layer.width;
   int height = layer.height;
   int color_components = layer.color_components;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;

  float s_L = 0.0f;
  float s_a = 0.0f;
  float s_b = 0.0f;

  struct mean_and_variance r ;
 
  size_t N = 0;
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       color_t rc, gc, bc;
       rc = image[idx];
       gc = image[idx+1];
       bc = image[idx+2];
       vec3 rgb = vec3_init(rc/255.0f, gc/255.0f, bc/255.0f);    
       vec3 LMS = RGBtoLMS(rgb);
       vec3 Lab = LMStoLab(LMS);
       s_L += Lab.x;
       s_a += Lab.y;
       s_b += Lab.z;
       N++;
    }
  }

  r.L_m = s_L / N;
  r.a_m = s_a / N;
  r.b_m = s_b / N;

  s_L = s_a = s_b = 0.0f;
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       color_t rc, gc, bc;
       rc = image[idx];
       gc = image[idx+1];
       bc = image[idx+2];
       vec3 rgb = vec3_init(rc/255.0f, gc/255.0f, bc/255.0f);    
       vec3 LMS = RGBtoLMS(rgb);
       vec3 Lab = LMStoLab(LMS);
       s_L += (Lab.x-r.L_m) * (Lab.x-r.L_m);
       s_a += (Lab.y-r.a_m) * (Lab.y-r.a_m);
       s_b += (Lab.z-r.b_m) * (Lab.z-r.b_m);
    }
  }
  r.L_s = sqrt(s_L / N);
  r.a_s = sqrt(s_a / N);
  r.b_s = sqrt(s_b / N);
  return r;
}

static void apply_reinhard2001(layer_t source, layer_t dest, rect_t source_zone, rect_t dest_zone, struct mean_and_variance src_stat, struct mean_and_variance dst_stat, float variance_coef, float mean_coef) {
   color_t *image = dest.image;
   int width = dest.width;
   int height = dest.height;
   int color_components = dest.color_components; 
  
  rect_t zone = dest_zone;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;

  vec3 k = vec3_init(
     1+((src_stat.L_s / dst_stat.L_s)-1)*variance_coef,
     1+((src_stat.a_s / dst_stat.a_s)-1)*variance_coef,
     1+((src_stat.b_s / dst_stat.b_s)-1)*variance_coef
  );
 vec3_info(k);
#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       color_t rc, gc, bc;
       rc = image[idx];
       gc = image[idx+1];
       bc = image[idx+2];

       vec3 rgb = vec3_init(rc/255.0f, gc/255.0f, bc/255.0f);    
       vec3 LMS = RGBtoLMS(rgb);
       vec3 Lab = LMStoLab(LMS);

       /**
        * Substract the mean from the coordinates
        */
       float L = Lab.x - dst_stat.L_m;
       float a = Lab.y - dst_stat.a_m;
       float b = Lab.z - dst_stat.b_m;

       /**
        * Scale the variance.
        */
       L *= k.x;
       a *= k.y;
       b *= k.z; 

       /**
        * Add the source mean alpha-blended with destination mean.
        */
       L += (1.0f - mean_coef) * dst_stat.L_m + mean_coef * src_stat.L_m;
       a += (1.0f - mean_coef) * dst_stat.a_m + mean_coef * src_stat.a_m;
       b += (1.0f - mean_coef) * dst_stat.b_m + mean_coef * src_stat.b_m;

 
       vec3 backToLMS = LabtoLMS(vec3_init((float)L, (float)a, (float)b));
       vec3 backToRGB = LMStoRGB(backToLMS); 

       image[idx] = COLOR_MAX * backToRGB.x;
       image[idx+1] = COLOR_MAX * backToRGB.y;
       image[idx+2] = COLOR_MAX * backToRGB.z;

       if (backToRGB.x > 1.0f) image[idx] = COLOR_MAX;
       if (backToRGB.y > 1.0f) image[idx+1] = COLOR_MAX;
       if (backToRGB.z > 1.0f) image[idx+2] = COLOR_MAX;
       if (backToRGB.x < 0.0f) image[idx] = 0;
       if (backToRGB.y < 0.0f) image[idx+1] = 0;
       if (backToRGB.z < 0.0f) image[idx+2] = 0;
    }
  }
}
