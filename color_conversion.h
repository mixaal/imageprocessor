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

#ifdef __cplusplus
}
#endif

#endif /* __IM_COLOR_CONVERSION_H__ */
