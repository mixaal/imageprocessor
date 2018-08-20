#include "common.h"

float clamp(float x, float minr, float maxr)
{
  if (x >= maxr ) return maxr;
  if (x <= minr ) return minr;
  return x;
}

vec3 blend2(vec3 left, vec3 right, float pos)
{
  vec3 v;
  //pos = saturatef(pos);
  v.x = left.x * (1-pos) + right.x * pos; 
  v.y = left.y * (1-pos) + right.y * pos; 
  v.z = left.z * (1-pos) + right.z * pos; 
  return v;
}

vec3 blend3(vec3 left, vec3 main, vec3 right, float pos)
{
  if (pos < 0) {
    return blend2(left, main, pos + 1);
  } else if (pos > 0) {
    return blend2(main, right, pos);
  } else {
    return main;
  }
}


vec3 vec3_mix(vec3 a, vec3 b, float k)
{
  vec3 r;
  r.x = mix(a.x, b.x, k);
  r.y = mix(a.y, b.y, k);
  r.z = mix(a.z, b.z, k);
  return r;
}

float mix(float a, float b, float k)
{
  return a + (b - a) * k;
}

vec3 vec3_init(float x, float y, float z) 
{
  vec3 v = {x, y, z};
  return v;
}

double saturate(double c)
{
  if (c<0.0) return 0.0;
  if (c>1.0) return 1.0;
  return c;
}

float saturatef(float c)
{
  if (c<0.0f) return 0.0f;
  if (c>1.0f) return 1.0f;
  return c;
}

color_t saturatec(color_t c)
{
  if (c<0) return 0;
  if (c>COLOR_MAX) return COLOR_MAX;
  return c;
}

float minf(float a, float b, float c)
{
   float m = a;
   if ( a > b ) m = b;
   if ( m > c ) m = c;
   return m;
}


float maxf(float a, float b, float c)
{
   float m = a;
   if ( a < b ) m = b;
   if ( m < c ) m = c;
   return m;
}


