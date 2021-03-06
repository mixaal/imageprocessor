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
  float I = 0.3f * color.x + 0.59f * color.y + 0.11f * color.z;
  if(I<0.0f) I=0.0f;
  if(I>1.0f) I=1.0f;
  return I;
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


// https://stackoverflow.com/questions/40017741/mathematical-conversion-srgb-and-adobergb

static float linear_sRGB(float c)
{
    if (c <= 0.04045)
        return c / 12.92f;
    else
        return powf((c + 0.055) / 1.055, 2.4);
}


//D65 whitepoint
vec3 sRGBtoXYZn(vec3 sRGB)
{
    float Rlin = linear_sRGB(sRGB.x / 255.0f);
    float Glin = linear_sRGB(sRGB.y / 255.0f);
    float Blin = linear_sRGB(sRGB.z / 255.0f);
    float Xn = Rlin * 0.4124f + Glin * 0.3576f + Blin * 0.1805f;
    float Yn = Rlin * 0.2126f + Glin * 0.7152f + Blin * 0.0722f;
    float Zn = Rlin * 0.0193f + Glin * 0.1192f + Blin * 0.9505f;
    return vec3_init(Xn, Yn, Zn);
}

static float gamma_sRGB(float c)
{
    if (c <= 0.0031308)
        return 12.92 * c;
    else
        return 1.055 * powf(c, 1/2.4) - 0.055;
}

//D65 whitepoint
vec3 XYZn_to_sRGB(vec3 XYZn)
{
    float Xn = XYZn.x;
    float Yn = XYZn.y;
    float Zn = XYZn.z;
    float Rlin = Xn * 3.2406255f + Yn *-1.5372080f + Zn *-0.4986286f;
    float Glin = Xn *-0.9689307f + Yn * 1.8757561f + Zn * 0.0415175f;
    float Blin = Xn * 0.0557101f + Yn *-0.2040211f + Zn * 1.0569959f;
    float R = round(255 * gamma_sRGB(Rlin));
    float G = round(255 * gamma_sRGB(Glin));
    float B = round(255 * gamma_sRGB(Blin));
    return vec3_init(R, G, B);
}

static float linear_AdobeRGB(float c)
{
    if (c <= 0.0) return 0.0;
    return powf(c, 2.19921875);
}

vec3 AdobeRGB_to_XYZn(vec3 AdobeRGB)
{
    float R = AdobeRGB.x;
    float G = AdobeRGB.y;
    float B = AdobeRGB.z;
    float Rlin = linear_AdobeRGB(R / 255.0);
    float Glin = linear_AdobeRGB(G / 255.0);
    float Blin = linear_AdobeRGB(B / 255.0);
    float Xn = Rlin * 0.57667f + Glin * 0.18556f + Blin * 0.18823f;
    float Yn = Rlin * 0.29734f + Glin * 0.62736f + Blin * 0.07529f;
    float Zn = Rlin * 0.02703f + Glin * 0.07069f + Blin * 0.99134f;
    return vec3_init(Xn, Yn, Zn);
}

static float gamma_AdobeRGB(float c)
{
    if (c <= 0.0) return 0.0;
    return powf(c, 1/2.19921875);
}

vec3 XYZn_to_AdobeRGB(vec3 XYZn)
{

    float Xn = XYZn.x;
    float Yn = XYZn.y;
    float Zn = XYZn.z;
    float Rlin = Xn * 2.04159f + Yn *-0.56501f + Zn *-0.34473f;
    float Glin = Xn *-0.96924f + Yn * 1.87597f + Zn * 0.04156f;
    float Blin = Xn * 0.01344f + Yn *-0.11836f + Zn * 1.01517f;
    float R = round(255 * gamma_AdobeRGB(Rlin));
    float G = round(255 * gamma_AdobeRGB(Glin));
    float B = round(255 * gamma_AdobeRGB(Blin));
    return vec3_init(R, G, B);
}



// Based on: Color Transfer between Images, Erik Reinhard, Michael Ashikhmin, Bruce Gooch, and Peter Shirley, 2001

vec3 RGBtoXYZ(vec3 RGB)
{
   vec3 XYZ = vec3_init(
      0.5141f*RGB.x + 0.3239f*RGB.y + 0.1604f*RGB.z,
      0.2651f*RGB.x + 0.6702f*RGB.y + 0.0641f*RGB.z,
      0.0241f*RGB.x + 0.1228f*RGB.y + 0.8444f*RGB.z
   );
   return XYZ;
}

vec3 RGBtoLMS(vec3 RGB)
{
  vec3 LMS = vec3_init(
      0.3811f*RGB.x + 0.5783f*RGB.y + 0.0402f*RGB.z,
      0.1967f*RGB.x + 0.7244f*RGB.y + 0.0782f*RGB.z,
      0.0241f*RGB.x + 0.1228f*RGB.y + 0.8444f*RGB.z
  );
  return LMS;
}

vec3 log10LMS(vec3 LMS)
{
  vec3 log_scale = vec3_init(
     log10f(LMS.x), log10f(LMS.y), log10f(LMS.z)
  );
  return log_scale;
}

vec3 pow10LMS(vec3 LMS)
{
  vec3 linear_scale = vec3_init(
     powf(10.0f, LMS.x), powf(10.0f, LMS.y), powf(10.0f, LMS.z)
  );
  return linear_scale;
}


vec3 LMStoLab(vec3 LMS)
{
  vec3 LAB = vec3_init(
      LMS.x/sqrtf(3) + LMS.y/sqrtf(3) + LMS.z/sqrtf(3),
      LMS.x/sqrtf(6) + LMS.y/sqrtf(6) - 2*LMS.z/sqrtf(6),
      LMS.x/sqrtf(2) - LMS.y/sqrtf(2)
  );
  return LAB;
}

vec3 LabtoLMS(vec3 Lab)
{
  vec3 LMS = vec3_init(
      Lab.x/sqrtf(3) + Lab.y/sqrtf(6) + Lab.z/sqrtf(2),
      Lab.x/sqrtf(3) + Lab.y/sqrtf(6) - Lab.z/sqrtf(2),
      Lab.x/sqrtf(3) - 2*Lab.y/sqrtf(6)
  );
  return LMS;
}

vec3 LMStoRGB(vec3 LMS)
{
  vec3 RGB = vec3_init(
        4.4679f * LMS.x - 3.5873f * LMS.y  + 0.1193f * LMS.z,
       -1.2186f * LMS.x + 2.3809f * LMS.y  - 0.1624f * LMS.z,
        0.0497f * LMS.x - 0.2439f * LMS.y  + 1.2045f * LMS.z
  );
  return RGB;
}

// calculate the perceptual distance between colors in CIELAB
// https://github.com/THEjoezack/ColorMine/blob/master/ColorMine/ColorSpaces/Comparisons/Cie94Comparison.cs

float delta_Lab(vec3 labA, vec3 labB){
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

/**
 * Compute perceptual color distance between src and dst.
 * 
 * @param src color in RGB space
 * @param dst color in RGB space
 * @returns perceptual color distance
 */
float color_distance(vec3 src, vec3 dst)
{
#define DELTA 15.0f
   vec3 src_Lab = LMStoLab(RGBtoLMS(src));
   vec3 dst_Lab = LMStoLab(RGBtoLMS(dst));
   float dx2 = pow(src_Lab.x - dst_Lab.x, 2);
   float dy2 = pow(src_Lab.y - dst_Lab.y, 2);
   float dz2 = pow(src_Lab.z - dst_Lab.z, 2);
   return 1 - exp(-sqrt(dx2+dy2+dz2)/DELTA);
   //return delta_Lab(src_Lab, dst_Lab);
}

