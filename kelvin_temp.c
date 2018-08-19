// ported by Renaud Bédard (@renaudbedard) from original code from Tanner Helland
// http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/

// color space functions translated from HLSL versions on Chilli Ant (by Ian Taylor)
// http://www.chilliant.com/rgb2hsv.html

// licensed and released under Creative Commons 3.0 Attribution
// https://creativecommons.org/licenses/by/3.0/


#include <math.h>
#include "kelvin_temp.h"
#include "color_conversion.h"

#define LUMINANCE_PRESERVATION 0.25

vec3 ColorTemperatureToRGB(float temperatureInKelvins)
{
  vec3 retColor;

  if (temperatureInKelvins<1000.0f) temperatureInKelvins = 1000.0f;
  if (temperatureInKelvins>40000.0f) temperatureInKelvins = 40000.0f;
  temperatureInKelvins /= 100.0f;
    
  if (temperatureInKelvins <= 66.0f)
    {
        retColor.x = 1.0;
        retColor.y = saturate(0.39008157876901960784 * log(temperatureInKelvins) - 0.63184144378862745098);
    }
    else
    {
        float t = temperatureInKelvins - 60.0;
        retColor.x = saturate(1.29293618606274509804 * pow(t, -0.1332047592));
        retColor.y = saturate(1.12989086089529411765 * pow(t, -0.0755148492));
    }
    
    if (temperatureInKelvins >= 66.0f)
        retColor.z = 1.0f;
    else if(temperatureInKelvins <= 19.0f)
        retColor.z = 0.0f;
    else
        retColor.z = saturate(0.54320678911019607843 * log(temperatureInKelvins - 10.0f) - 1.19625408914);

    return retColor;
}


void kelvin_temperature(layer_t layer, float colorTempK, float factor, rect_t zone) {
  color_t *image = layer.image;
  int width = layer.width;
  int height = layer.height;
  int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;

  vec3 colorTempRGB = ColorTemperatureToRGB(colorTempK);

  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       float nr = (float) image[idx] / COLOR_MAX;
       float ng = (float) image[idx+1] / COLOR_MAX;
       float nb = (float) image[idx+2] / COLOR_MAX;


       float originalLuminance = luminance(vec3_init(nr, ng, nb));
       vec3 blended = vec3_init( 
            mix(nr, nr * colorTempRGB.x, factor),
            mix(ng, ng * colorTempRGB.y, factor),
            mix(nb, nb * colorTempRGB.z, factor)
       );
       vec3 resultHSL = RGBtoHSL(blended);
       vec3 luminancePreservedRGB = HSLtoRGB(vec3_init(resultHSL.x, resultHSL.y, originalLuminance));        
       vec3 fragColor = vec3_mix(blended, luminancePreservedRGB, LUMINANCE_PRESERVATION);
       nr = COLOR_MAX * fragColor.x;
       ng = COLOR_MAX * fragColor.y;
       nb = COLOR_MAX * fragColor.z;
       if (nr > COLOR_MAX) nr = COLOR_MAX;
       if (ng > COLOR_MAX) ng = COLOR_MAX;
       if (nb > COLOR_MAX) nb = COLOR_MAX;
       if (nr < 0.0f) nr = 0.0f;
       if (ng < 0.0f) ng = 0.0f;
       if (nb < 0.0f) nb = 0.0f;
       image[idx] = (color_t) nr;
       image[idx+1] = (color_t) ng;
       image[idx+2] = (color_t) nb;
    }
  }
}
