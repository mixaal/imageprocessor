#ifndef __IM_KELVIN_TEMP_H__
#define __IM_KELVIN_TEMP_H__ 1 

#include "common.h"
#include "layer.h"

#ifdef __cplusplus
extern "C" {
#endif

vec3 ColorTemperatureToRGB(float temperatureInKelvins);
void kelvin_temperature(layer_t layer, float colorTempK, float factor, float luminance_preservation, rect_t zone);


#ifdef __cplusplus
}
#endif

#endif /* __IM_KELVIN_TEMP_H__ */
