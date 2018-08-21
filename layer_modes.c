#include "common.h"
#include "layer_modes.h"


vec3 blend_screen(vec3 a, vec3 b, float opacity) 
{
  opacity = saturatef(opacity);
  vec3_clamp(a, 0.0f, 1.0f);
  vec3_clamp(b, 0.0f, 1.0f);
  vec3 one = vec3_init(1.0f, 1.0f, 1.0f);
  vec3 output = vec3_sub(
      one, 
      vec3_dot(
        vec3_sub(one, a),
        vec3_sub(one, b)
      )
  );
  vec3_clamp(output, 0.0f, 1.0f);
  return blend2( b, output, opacity );
}


vec3 blend_multiply(vec3 a, vec3 b, float opacity) 
{
  opacity = saturatef(opacity);
  vec3_clamp(a, 0.0f, 1.0f);
  vec3_clamp(b, 0.0f, 1.0f);
  vec3 output = vec3_dot(a, b);
  vec3_clamp(output, 0.0f, 1.0f);
  return blend2( b, output, opacity );
}

vec3 blend_normal(vec3 a, vec3 b, float opacity)
{
  opacity = saturatef(opacity);
  vec3_clamp(a, 0.0f, 1.0f);
  vec3_clamp(b, 0.0f, 1.0f);
  return blend2( b, a, opacity );
}
