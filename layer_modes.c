#include "common.h"
#include "layer_modes.h"
#include "color_conversion.h"
#include <stdlib.h>
#include <stdio.h>

vec3 blend_difference(vec3 a, vec3 b, float opacity)
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);

  vec3 output;
  output.x = (a.x > b.x) ? a.x - b.x : b.x - a.x;
  output.y = (a.y > b.y) ? a.y - b.y : b.y - a.y;
  output.z = (a.z > b.z) ? a.z - b.z : b.z - a.z;

  return output;
}

vec3 blend_lighten_only(vec3 a, vec3 b, float opacity)
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);

  vec3 output;
  output.x = max(a.x, b.x);
  output.y = max(a.y, b.y);
  output.z = max(a.z, b.z);

  return output;
}

vec3 blend_darken_only(vec3 a, vec3 b, float opacity)
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);

  vec3 output;
  output.x = min(a.x, b.x);
  output.y = min(a.y, b.y);
  output.z = min(a.z, b.z);

  return output;
}

vec3 blend_substraction(vec3 a, vec3 b, float opacity)
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);

  vec3 output = vec3_sub(a, b);
  output = vec3_clamp(output, 0.0f, 1.0f);
  return blend2( b, output, opacity );
}

// b + 2a - base
vec3 blend_linear_light(vec3 a, vec3 b, float opacity)
{
  vec3 base = vec3_init(0.5f, 0.5f, 0.5f);
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);
  vec3 output = vec3_sub(vec3_add(b, vec3_multiply(a, 2.0f)), base);
  output=vec3_clamp(output, 0.0f, 1.0f);
  return blend2( b, output, opacity );
}

// known also as linear dodge
vec3 blend_addition(vec3 a, vec3 b, float opacity)
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);
  vec3 output = vec3_add(a, b);
  output=vec3_clamp(output, 0.0f, 1.0f);
  return blend2( b, output, opacity );
}

vec3 blend_overlay(vec3 a, vec3 b, float opacity)
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);
  vec3 one = vec3_init(1.0f, 1.0f, 1.0f);
  float Iv = to_gray(a);
  if (Iv < 0.5f) return blend_multiply(a, b, opacity);
  vec3 output = vec3_sub(
      one, 
      vec3_multiply(vec3_dot(
        vec3_sub(one, a),
        vec3_sub(one, b)
      ), 2.0f)
  );
  output = vec3_clamp(output, 0.0f, 1.0f);
  return blend2( b, output, opacity );

}

vec3 blend_screen(vec3 a, vec3 b, float opacity) 
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);
  vec3 one = vec3_init(1.0f, 1.0f, 1.0f);
  vec3 output = vec3_sub(
      one, 
      vec3_dot(
        vec3_sub(one, a),
        vec3_sub(one, b)
      )
  );
  output = vec3_clamp(output, 0.0f, 1.0f);
  return blend2( b, output, opacity );
}


vec3 blend_multiply(vec3 a, vec3 b, float opacity) 
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);
  vec3 output = vec3_dot(a, b);
  output = vec3_clamp(output, 0.0f, 1.0f);
  return blend2( b, output, opacity );
}

vec3 blend_normal(vec3 a, vec3 b, float opacity)
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);
  return blend2( b, a, opacity );
}

vec3 blend_color_dodge(vec3 a, vec3 b, float opacity)
{
  opacity = saturatef(opacity);
  a = vec3_clamp(a, 0.0f, 1.0f);
  b = vec3_clamp(b, 0.0f, 1.0f);

  // Target / (1-Blend)    
  vec3 one = vec3_init(1.0f, 1.0f, 1.0f);
  vec3 one_minus_blend = vec3_sub(one, vec3_multiply(b, opacity));
  vec3 result = vec3_div(a, one_minus_blend);
  return vec3_clamp(result, 0.0f, 1.0f);
}
