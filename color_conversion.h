#ifndef __IM_COLOR_CONVERSION_H__
#define __IM_COLOR_CONVERSION_H__ 1

#include "common.h"


#ifdef __cplusplus
extern "C" {
#endif
float to_gray(vec3 color) ;
float luminance(vec3 color);
vec3 HUEtoRGB(float H);
vec3 HSLtoRGB(vec3 HSL);
vec3 RGBtoHCV(vec3 RGB);
vec3 RGBtoHSL(vec3 RGB);
vec3 LMStoRGB(vec3 LMS);
vec3 LabtoLMS(vec3 Lab);
vec3 LMStoLab(vec3 LMS);
vec3 log10LMS(vec3 LMS);
vec3 pow10LMS(vec3 LMS);
vec3 RGBtoLMS(vec3 RGB);
vec3 RGBtoXYZ(vec3 RGB);

float deltaE(vec3 labA, vec3 labB);
vec3 RGB2Lab(vec3 rgb);
#ifdef __cplusplus
}
#endif

#endif /* __IM_COLOR_CONVERSION_H__ */
