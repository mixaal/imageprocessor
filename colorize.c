#include <math.h>
#include "common.h"
#include "color_conversion.h"
#include "colorize.h"

// Adapted from: https://stackoverflow.com/questions/4404507/algorithm-for-hue-saturation-adjustment-layer-from-photoshop

static vec3 colorize_magic(vec3 pixel, vec3 hueRGB, float saturation, float lightness, _Bool preserve_original_pixel_saturation) ;

/**
 * Colorize image: 
 *    - hueRGB: Color: [hue, 1, 1] -> [r, g, b]
 *    - saturation : [0, 1]
 *    - lightness : [-1, 1]
 */
void colorize(layer_t layer, vec3 hueRGB, float saturation, float lightness, levels_t affect_levels, _Bool preserve_original_pixel_saturation, rect_t zone) {
  color_t *image = layer.image;
  int width = layer.width;
  int height = layer.height;
  int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       float r, g, b;
       r = (float)image[idx] ;
       g = (float)image[idx+1] ;
       b = (float)image[idx+2] ;

       float pixel_intensity = to_gray(vec3_init(r, g, b)) / COLOR_MAX ;
       if (affect_levels == SHADOWS && pixel_intensity > SHADOWS_MAX_INTENSITY) continue;
       if (affect_levels == HIGHLIGHTS && pixel_intensity < HIGHLIGHTS_MIN_INTENSITY) continue;
       if (affect_levels == MIDTONES && (pixel_intensity <= SHADOWS_MAX_INTENSITY || pixel_intensity >= HIGHLIGHTS_MIN_INTENSITY)) continue;

       vec3 v= colorize_magic(vec3_init(r, g, b), hueRGB, saturation, lightness, preserve_original_pixel_saturation);
       float nr = v.x;
       float ng = v.y;
       float nb = v.z;

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

static vec3 color_black()
{
  return vec3_init(0, 0, 0);
}

static vec3 color_white()
{
  return vec3_init(COLOR_MAX, COLOR_MAX, COLOR_MAX);
}

static vec3 colorize_magic(vec3 pixel, vec3 hueRGB, float saturation, float lightness, _Bool preserve_original_pixel_saturation) 
{

  vec3 normalized_pixel = pixel;
  normalized_pixel.x /= (float) COLOR_MAX;
  normalized_pixel.y /= (float) COLOR_MAX;
  normalized_pixel.z /= (float) COLOR_MAX;
  vec3 HSL = RGBtoHSL(normalized_pixel);
  
  vec3 color = blend2(vec3_init(COLOR_MAX/2, COLOR_MAX/2, COLOR_MAX/2), hueRGB, (preserve_original_pixel_saturation) ? HSL.y : saturation);

  float sat = HSL.y * (lightness<.5?lightness:1-lightness);
  float value = HSL.z + sat;
  if (lightness <= -1) {
    return color_black();
  } else if (lightness >= 1) {
    return color_white();
  } else if (lightness >= 0) {
    return blend3(color_black(), color, color_white(), 2 * (1 - lightness) * (value - 1) + 1);
  } else {
    return blend3(color_black(), color, color_white(),  2 * (1 + lightness) * (value) - 1);
  }
}
