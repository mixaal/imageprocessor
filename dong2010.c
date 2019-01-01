/**
 *  Fast Local Color Transfer via Dominant Colors Mapping, 2010
 *
 *  Weiming Dong, Guanbo Bao (1),(2), Xiaopeng Zhang (1), Jean-Claude Paul (2)
 * 
 * (1) LIAMA-NLPR-Digital Content Technology Research Center, CAS Institute of Automation, China, (2) INRIA, France
 */
#include "dong2010.h"
#include "color_conversion.h"
#include "permgen.h"
#include "kmeans.h"

#include <math.h>
#include <float.h>

#define DOMINANT_COLORS_NO 5
#define DELTA 15.0f

static float color_distance(vec3 src, vec3 dst);

void apply_color_dong2010(layer_t source, layer_t dest, rect_t source_zone, rect_t dest_zone) {
 

   /**
    * First, compute dominant colors in both source and destination.
    */
    vec3 source_dominant_colors[DOMINANT_COLORS_NO];
    vec3 dest_dominant_colors[DOMINANT_COLORS_NO];
    vec3 source_variance[DOMINANT_COLORS_NO];
    vec3 dest_variance[DOMINANT_COLORS_NO];
    kmeans(source, source.zone, DOMINANT_COLORS_NO, source_dominant_colors, NULL, source_variance);
    kmeans(dest, dest.zone, DOMINANT_COLORS_NO, dest_dominant_colors, NULL, dest_variance);

    printf("source dominant colors\n");
    for(int j=0; j<DOMINANT_COLORS_NO; j++) vec3_info(source_dominant_colors[j]);
    printf("dest dominant colors\n");
    for(int j=0; j<DOMINANT_COLORS_NO; j++) vec3_info(dest_dominant_colors[j]);
    printf("source variance\n");
    for(int j=0; j<DOMINANT_COLORS_NO; j++) vec3_info(source_variance[j]);
    printf("dest variance\n");
    for(int j=0; j<DOMINANT_COLORS_NO; j++) vec3_info(dest_variance[j]);

    /**
     * Find best mapping from source to destination.
     */
    int mapping[DOMINANT_COLORS_NO];
    
    permgen_t p = permgen_init(DOMINANT_COLORS_NO);
    float best_error = FLT_MAX;
    while(permgen_has_more(&p)) {
       int *perm = permgen_get_next(&p);
       float error = 0.0f;
       for(int i=0; i<DOMINANT_COLORS_NO; i++) {
          error += color_distance(source_dominant_colors[perm[i]], dest_dominant_colors[i]);
       }
       if( error < best_error ) {
          best_error = error;
          /**
           * Preserve the best mapping so far.
           */
          for(int i=0; i<DOMINANT_COLORS_NO; i++) mapping[i] = perm[i];
       }
    }

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
  
       float Psum = 0.0f;
       for(int j=0; j<DOMINANT_COLORS_NO; j++) {
         float dx = pow(Lab.x - dest_dominant_colors[j].x, 2.0f) / (2*dest_variance[j].x);
         float dy = pow(Lab.y - dest_dominant_colors[j].y, 2.0f) / (2*dest_variance[j].y);
         float dz = pow(Lab.z - dest_dominant_colors[j].z, 2.0f) / (2*dest_variance[j].z);
         Psum += exp(-dx -dy -dz) ;
       }
       float Pxy[DOMINANT_COLORS_NO];
       for(int j=0; j<DOMINANT_COLORS_NO; j++) {
         float dx = pow(Lab.x - dest_dominant_colors[j].x, 2.0f) / (2*dest_variance[j].x);
         float dy = pow(Lab.y - dest_dominant_colors[j].y, 2.0f) / (2*dest_variance[j].y);
         float dz = pow(Lab.z - dest_dominant_colors[j].z, 2.0f) / (2*dest_variance[j].z);
         Pxy[j] = exp(-dx -dy -dz) / Psum ;
         //printf("Pxy[%d]=%f\n", j, Pxy[j]);
       }
       vec3 Io = vec3_init(0, 0, 0);
       for(int j=0; j<DOMINANT_COLORS_NO; j++) {
          vec3 sigma_src = source_variance[mapping[j]];
          vec3 sigma_dst = dest_variance[j];
          if (sigma_src.x < 0 || sigma_src.y < 0 || sigma_src.z < 0) continue;
          if (sigma_dst.x < 0 || sigma_dst.y < 0 || sigma_dst.z < 0) continue;
          Pxy[j] = 0.2f;
          Io = vec3_add(Io, vec3_init(
             Pxy[j] * (sqrtf(sigma_src.x)/sqrtf(sigma_dst.x) * (Lab.x - dest_dominant_colors[j].x) + source_dominant_colors[mapping[j]].x),
             Pxy[j] * (sqrtf(sigma_src.y)/sqrtf(sigma_dst.y) * (Lab.y - dest_dominant_colors[j].y) + source_dominant_colors[mapping[j]].y),
             Pxy[j] * (sqrtf(sigma_src.z)/sqrtf(sigma_dst.z) * (Lab.z - dest_dominant_colors[j].z) + source_dominant_colors[mapping[j]].z)
          ));
       }
       
       vec3 backToRGB = LMStoRGB(LabtoLMS(Io));
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

static float color_distance(vec3 src, vec3 dst)
{
   vec3 src_Lab = LMStoLab(RGBtoLMS(src));
   vec3 dst_Lab = LMStoLab(RGBtoLMS(dst));
   float dx2 = pow(src_Lab.x - dst_Lab.x, 2);
   float dy2 = pow(src_Lab.y - dst_Lab.y, 2);
   float dz2 = pow(src_Lab.z - dst_Lab.z, 2);
   return 1 - exp(-sqrt(dx2+dy2+dz2)/DELTA);
}
