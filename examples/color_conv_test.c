#include <color_conversion.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

int main(void)
{

  float min_L=FLT_MAX, max_L=FLT_MIN, min_a=FLT_MAX, max_a=FLT_MIN, min_b=FLT_MAX, max_b=FLT_MIN;
  for (unsigned int i=0; i<=16777216; i++) {
    unsigned char r = (i&0xff0000) >> 16;
    unsigned char g = (i&0xff00) >> 8;
    unsigned char b = (i&0xff);

    vec3 rgb = vec3_init(r/255.0f, g/255.0f, b/255.0f);

    //vec3 LMS = log10LMS(RGBtoLMS(rgb));
    vec3 LMS = RGBtoLMS(rgb);
    vec3 Lab = LMStoLab(LMS);
    if (Lab.x < min_L) min_L = Lab.x;
    if (Lab.x > max_L) max_L = Lab.x;

    if (Lab.y < min_a) min_a = Lab.y;
    if (Lab.y > max_a) max_a = Lab.y;

    if (Lab.z < min_b) min_b = Lab.z;
    if (Lab.z > max_b) max_b = Lab.z;

    //vec3 backToLMS = pow10LMS(LabtoLMS(Lab));
    vec3 backToLMS = LabtoLMS(Lab);
    vec3 backToRGB = LMStoRGB(backToLMS); 
    vec3 diff = vec3_add(rgb, vec3_multiply(backToRGB, -1.0f));
    if (diff.x > 0.001f || diff.y > 0.001f || diff.z > 0.001f) {
       printf("FAIL @ i=%08x\n", i);
       vec3_info(rgb); 
       vec3_info(backToRGB); 
       exit(-1);
    }
  }

  printf("L [%f, %f]   a [%f, %f]    b [%f, %f]\n", min_L, max_L, min_a, max_a, min_b, max_b);
  return 0;
}
