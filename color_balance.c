#include "common.h"
#include "color_conversion.h"
#include "color_balance.h"
#include "colorize.h"

static float hue_red(void);
static float hue_yellow(void);
static float hue_green(void);
static float hue_blue(void);
static float hue_cyan(void);
static float hue_magenta(void);

static const float one_twelth = 1.0f / 12;

static _Bool inside(float color, float hue)
{
   float left = hue - one_twelth;
   float right = hue + one_twelth;
   if (left < 0.0f) left += 1.0f;
   if (right > 1.0f) right -= 1.0f;

   return (color >= left && color < right);
}

// Colorize into balance cyan-red, magenta-green, yellow-blue, -1,1
void adjust_color_balance(
  layer_t layer, 
  float cyan_red_coef,
  float magenta_green_coef,
  float yellow_blue_coef,
  float saturation,
  float lightness,
  levels_t level,
  rect_t zone) {
  cyan_red_coef = clamp(cyan_red_coef, -1 , 1);
  magenta_green_coef = clamp(magenta_green_coef, -1, 1);
  yellow_blue_coef = clamp(yellow_blue_coef, -1, 1);

  fprintf(stderr, "cyan_red_coef=%f\nmagenta_green_coef=%f\nyellow_blue_coef=%f\n", cyan_red_coef, magenta_green_coef, yellow_blue_coef);
  // 0 .. 1 range
  cyan_red_coef *= 0.5f;
  cyan_red_coef += 0.5f;

  magenta_green_coef *= 0.5f;
  magenta_green_coef += 0.5f;

  yellow_blue_coef *= 0.5f;
  yellow_blue_coef += 0.5f;

  fprintf(stderr, "cyan_red_coef=%f\nmagenta_green_coef=%f\nyellow_blue_coef=%f\n", cyan_red_coef, magenta_green_coef, yellow_blue_coef);

  vec3 base = vec3_init(0.5f, 0.5f, 0.5f);
  vec3 cyan_red_vector =  vec3_sub( vec3_init(cyan_red_coef, 1- cyan_red_coef, 1- cyan_red_coef), base ) ;
  vec3 magenta_green_vector = vec3_sub( vec3_init(1-magenta_green_coef, magenta_green_coef, 1-magenta_green_coef), base);
  vec3 yellow_blue_vector =  vec3_sub( vec3_init(1-yellow_blue_coef, 1-yellow_blue_coef, yellow_blue_coef), base);

  fprintf(stderr, "cyan_red_vector=[%f %f %f]\n", cyan_red_vector.x, cyan_red_vector.y, cyan_red_vector.z);
  fprintf(stderr, "magenta_green_coef=[%f %f %f]\n", magenta_green_vector.x, magenta_green_vector.y, magenta_green_vector.z);
  fprintf(stderr, "yellow_blue_coef=[%f %f %f]\n", yellow_blue_vector.x, yellow_blue_vector.y, yellow_blue_vector.z);

  
  vec3 final_color = vec3_add3( cyan_red_vector, magenta_green_vector, yellow_blue_vector );

  final_color.x = COLOR_MAX * saturatef(final_color.x);
  final_color.y = COLOR_MAX * saturatef(final_color.y);
  final_color.z = COLOR_MAX * saturatef(final_color.z);

  fprintf(stderr, "final_color=[%f %f %f]\n", final_color.x, final_color.y, final_color.z);
  colorize( layer, final_color, saturation, lightness, level, False, zone );
}

/** 
 * Adjust color saturation. Range for color saturation: [0, 1]
 */ 
void adjust_color_saturation(
  layer_t layer, 
  float reds_sat, float yellows_sat, float greens_sat, float cyans_sat, float blues_sat, float magentas_sat, 
  float reds_light, float yellows_light, float greens_light, float cyans_light, float blues_light, float magentas_light, 
  rect_t zone) {
  reds_sat = clamp(reds_sat, -1 , 1);
  yellows_sat = clamp(yellows_sat, -1, 1);
  greens_sat = clamp(greens_sat, -1, 1);
  cyans_sat = clamp(cyans_sat, -1, 1);
  blues_sat = clamp(blues_sat, -1, 1);
  magentas_sat = clamp(magentas_sat, -1, 1);

  //printf("reds=%f\nyellows=%f\n,greens=%f\ncyans=%f\nblues=%f\nmagentas=%f\n", reds, yellows, greens, cyans, blues, magentas);
  //printf("redsH=%f\nyellowsH=%f\n,greensH=%f\ncyansH=%f\nbluesH=%f\nmagentasH=%f\n", hue_red(), hue_yellow(), hue_green(), hue_cyan(), hue_blue(), hue_magenta());

  color_t *image = layer.image;
  int width = layer.width;
  int height = layer.height;
  int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       float r, g, b;
       r = (float)image[idx] / COLOR_MAX;
       g = (float)image[idx+1] / COLOR_MAX;
       b = (float)image[idx+2] / COLOR_MAX;

       vec3 HSL = RGBtoHSL(vec3_init(r, g, b));

       if (inside(HSL.x, hue_red())) {
         HSL.y += reds_sat;
         HSL.z += reds_light;
       }
       
       if (inside(HSL.x, hue_yellow())) {
         HSL.y += yellows_sat;
         HSL.z += yellows_light;
       }
       if (inside(HSL.x, hue_green())) {
         HSL.y += greens_sat;
         HSL.z += greens_light;
       }
       if (inside(HSL.x, hue_cyan())) {
         HSL.y += cyans_sat;
         HSL.z += cyans_light;
       }
       if (inside(HSL.x, hue_blue())) {
         HSL.y += blues_sat;
         HSL.z += blues_light;
       }
       if (inside(HSL.x, hue_magenta())) {
         HSL.y += magentas_sat;
         HSL.z += magentas_light;
       }

       HSL.y = saturatef(HSL.y);
       HSL.z = saturatef(HSL.z);
       vec3 newRGB = HSLtoRGB(HSL);
       
       float nr = COLOR_MAX * newRGB.x;
       float ng = COLOR_MAX * newRGB.y;
       float nb = COLOR_MAX * newRGB.z;
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

static float hue_red(void) {
   vec3 v = RGBtoHSL(vec3_init(1.0f, 0.0f, 0.0f));
   return v.x;
}

static float hue_yellow(void) {
   return RGBtoHSL(vec3_init(1.0f, 1.0f, 0.0f)).x;
}

static float hue_green(void) {
   return RGBtoHSL(vec3_init(0.0f, 1.0f, 0.0f)).x;
}

static float hue_blue(void) {
   return RGBtoHSL(vec3_init(0.0f, 0.0f, 1.0f)).x;
}

static float hue_cyan(void) {
   return RGBtoHSL(vec3_init(0.0f, 1.0f, 1.0f)).x;
}

static float hue_magenta(void) {
   return RGBtoHSL(vec3_init(1.0f, 0.0f, 1.0f)).x;
}
