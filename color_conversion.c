// ported by Renaud Bédard (@renaudbedard) from original code from Tanner Helland
// http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/

// color space functions translated from HLSL versions on Chilli Ant (by Ian Taylor)
// http://www.chilliant.com/rgb2hsv.html

// licensed and released under Creative Commons 3.0 Attribution
// https://creativecommons.org/licenses/by/3.0/

// playing with this value tweaks how dim or bright the resulting image is
#define LUMINANCE_PRESERVATION 0.75

#define EPSILON 1e-10

#include "color_conversion.h"
#include <math.h>

float to_gray(vec3 color) 
{
  return 0.3f * color.x + 0.59f * color.y + 0.11f * color.z;
}

float luminance(vec3 color)
{
  float fmin = minf(color.x, color.y, color.z);
  float fmax = maxf(color.x, color.y, color.z);
  return (fmax + fmin) / COLOR_MAX / 2.0f;
}

vec3 HUEtoRGB(float H)
{
    float R = saturatef(fabs(H * 6.0f - 3.0f) - 1.0f);
    float G = saturatef(2.0f - fabs(H * 6.0f - 2.0f));
    float B = saturatef(2.0f - fabs(H * 6.0f - 4.0f));
    vec3 r = {R, G, B};
    return r;
}

vec3 HSLtoRGB(vec3 HSL)
{
    vec3 RGB = HUEtoRGB(HSL.x);
    float C = (1.0f - fabs(2.0f * HSL.z - 1.0f)) * HSL.y;
    RGB.x -= 0.5f;
    RGB.y -= 0.5f;
    RGB.z -= 0.5f;
 
    RGB.x *= C;
    RGB.y *= C;
    RGB.z *= C;

    RGB.x += HSL.z;
    RGB.y += HSL.z;
    RGB.z += HSL.z;

    return RGB;

}
 
vec3 RGBtoHCV(vec3 RGB)
{
    // Based on work by Sam Hocevar and Emil Persson
    vec4 P, Q;
    if (RGB.y < RGB.z) {
       P.x = RGB.z;
       P.y = RGB.y;
       P.z = -1.0f;
       P.w = 2.0f/3.0f;
    } else {
       P.x = RGB.y;
       P.y = RGB.z;
       P.z = 0.0f;
       P.w = -1.0f/3.0f;
    }
    if (RGB.x < P.x) {
       Q.x = P.x;
       Q.y = P.y;
       Q.z = P.w;
       Q.w = RGB.x;
    } else {
       Q.x = RGB.x;
       Q.y = P.y;
       Q.z = P.z;
       Q.w = P.x;
    }
    float C = Q.x - min(Q.w, Q.y);
    float H = fabs((Q.w - Q.y) / (6.0f * C + EPSILON) + Q.z);
    return vec3_init(H, C, Q.x);
}

vec3 RGBtoHSL(vec3 RGB)
{
    vec3 HCV = RGBtoHCV(RGB);
    float L = HCV.z - HCV.y * 0.5f;
    float S = HCV.y / (1.0f - fabs(L * 2.0f - 1.0f) + EPSILON);
    return vec3_init(HCV.x, S, L);
}

