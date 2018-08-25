#include "common.h"
#include "color_conversion.h"
#include "color_balance.h"
#include "colorize.h"

#define GIMP_TRANSFER_SHADOWS 0
#define GIMP_TRANSFER_MIDTONES 1
#define GIMP_TRANSFER_HIGHLIGHTS 2

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

// Borrowed from gimp:
//    https://github.com/GNOME/gimp/blob/master/app/operations/gimpoperationcolorbalance.c
static float
gimp_operation_color_balance_map (float  value,
                                  double lightness,
                                  double shadows,
                                  double midtones,
                                  double highlights)
{
  /* Apply masks to the corrections for shadows, midtones and
   * highlights so that each correction affects only one range.
   * Those masks look like this:
   *     ‾\___
   *     _/‾\_
   *     ___/‾
   * with ramps of width a at x = b and x = 1 - b.
   *
   * The sum of these masks equals 1 for x in 0..1, so applying the
   * same correction in the shadows and in the midtones is equivalent
   * to applying this correction on a virtual shadows_and_midtones
   * range.
   */
  static const double a = 0.25, b = 0.333, scale = 0.7;

  shadows    *= clamp((lightness - b) / -a + 0.5, 0, 1) * scale;
  midtones   *= clamp ((lightness - b) /  a + 0.5, 0, 1) *
                clamp ((lightness + b - 1) / -a + 0.5, 0, 1) * scale;
  highlights *= clamp ((lightness + b - 1) /  a + 0.5, 0, 1) * scale;

  value += shadows;
  value += midtones;
  value += highlights;
  value = clamp(value, 0.0, 1.0);

  return value;
}


// Adjust color balance cyan-red, magenta-green, yellow-blue, -1,1
void adjust_color_balance(
  layer_t layer, 
  float cyan_red_coef[3],
  float magenta_green_coef[3],
  float yellow_blue_coef[3],
  rect_t zone) {

  for(int i=0; i<3; i++) {
    cyan_red_coef[i] = clamp(cyan_red_coef[i], -1 , 1);
    magenta_green_coef[i] = clamp(magenta_green_coef[i], -1, 1);
    yellow_blue_coef[i] = clamp(yellow_blue_coef[i], -1, 1);
    fprintf(stderr, "cyan_red_coef=%f\nmagenta_green_coef=%f\nyellow_blue_coef=%f\n", cyan_red_coef[i], magenta_green_coef[i], yellow_blue_coef[i]);
  }
  
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
       r = (float)image[idx] / COLOR_MAX;
       g = (float)image[idx+1] / COLOR_MAX;
       b = (float)image[idx+2] / COLOR_MAX;

       vec3 HSL = RGBtoHSL(vec3_init(r, g, b));

       float r_n = gimp_operation_color_balance_map (r, HSL.z,
                                              cyan_red_coef[GIMP_TRANSFER_SHADOWS],
                                              cyan_red_coef[GIMP_TRANSFER_MIDTONES],
                                              cyan_red_coef[GIMP_TRANSFER_HIGHLIGHTS]);

       float g_n = gimp_operation_color_balance_map (g, HSL.z,
                                              magenta_green_coef[GIMP_TRANSFER_SHADOWS],
                                              magenta_green_coef[GIMP_TRANSFER_MIDTONES],
                                              magenta_green_coef[GIMP_TRANSFER_HIGHLIGHTS]);

       float b_n = gimp_operation_color_balance_map (b, HSL.z,
                                              yellow_blue_coef[GIMP_TRANSFER_SHADOWS],
                                              yellow_blue_coef[GIMP_TRANSFER_MIDTONES],
                                              yellow_blue_coef[GIMP_TRANSFER_HIGHLIGHTS]);
       image[idx] = COLOR_MAX * r_n;
       image[idx+1] = COLOR_MAX * g_n;
       image[idx+2] = COLOR_MAX * b_n;
       if (r_n > 1.0f) image[idx] = COLOR_MAX;
       if (g_n > 1.0f) image[idx+1] = COLOR_MAX;
       if (b_n > 1.0f) image[idx+2] = COLOR_MAX;
       if (r_n < 0.0f) image[idx] = 0;
       if (g_n < 0.0f) image[idx+1] = 0;
       if (b_n < 0.0f) image[idx+2] = 0;
      
    }
  }

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
