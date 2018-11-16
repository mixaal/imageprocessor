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
  return (fmax + fmin) / 2.0f;
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


// Adapted from: https://github.com/antimatter15/rgb-lab/blob/master/color.js
vec3 RGB2Lab(vec3 rgb)
{
  float r = rgb.x,
      g = rgb.y,
      b = rgb.z,
      x, y, z;

  r = (r > 0.04045) ? powf((r + 0.055) / 1.055, 2.4) : r / 12.92;
  g = (g > 0.04045) ? powf((g + 0.055) / 1.055, 2.4) : g / 12.92;
  b = (b > 0.04045) ? powf((b + 0.055) / 1.055, 2.4) : b / 12.92;

  x = (r * 0.4124 + g * 0.3576 + b * 0.1805) / 0.95047;
  y = (r * 0.2126 + g * 0.7152 + b * 0.0722) / 1.00000;
  z = (r * 0.0193 + g * 0.1192 + b * 0.9505) / 1.08883;

  x = (x > 0.008856) ? powf(x, 1/3) : (7.787 * x) + 16/116;
  y = (y > 0.008856) ? powf(y, 1/3) : (7.787 * y) + 16/116;
  z = (z > 0.008856) ? powf(z, 1/3) : (7.787 * z) + 16/116;

  vec3 result = {(116 * y) - 16, 500 * (x - y), 200 * (y - z)};

  return result;
}

// calculate the perceptual distance between colors in CIELAB
// https://github.com/THEjoezack/ColorMine/blob/master/ColorMine/ColorSpaces/Comparisons/Cie94Comparison.cs

float deltaE(vec3 labA, vec3 labB){
  float deltaL = labA.x - labB.x;
  float deltaA = labA.y - labB.y;
  float deltaB = labA.z - labB.z;
  float c1 = sqrtf(labA.y * labA.y + labA.z * labA.z);
  float c2 = sqrtf(labB.y * labB.y + labB.z * labB.z);
  float deltaC = c1 - c2;
  float deltaH = deltaA * deltaA + deltaB * deltaB - deltaC * deltaC;
  deltaH = deltaH < 0 ? 0 : sqrtf(deltaH);
  float sc = 1.0 + 0.045 * c1;
  float sh = 1.0 + 0.015 * c1;
  float deltaLKlsl = deltaL / (1.0);
  float deltaCkcsc = deltaC / (sc);
  float deltaHkhsh = deltaH / (sh);
  float i = deltaLKlsl * deltaLKlsl + deltaCkcsc * deltaCkcsc + deltaHkhsh * deltaHkhsh;
  return i < 0 ? 0 : sqrtf(i);
}
