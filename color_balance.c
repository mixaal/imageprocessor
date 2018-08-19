#include "common.h"
#include "color_conversion.h"
#include "color_balance.h"

#define ADJUST_COLOR_MIN -200
#define ADJUST_COLOR_MAX 300
#define TGT_BASE_MIN -2.5f
#define TGT_BASE_MAX 2.5f
static float hue_red(void);
static float hue_yellow(void);
static float hue_green(void);
static float hue_blue(void);
static float hue_cyan(void);
static float hue_magenta(void);

static float remap(float);
/** 
 * Adjust color balance. Range for wights [-200, 300]
 * 
 * Adapted from : https://dsp.stackexchange.com/questions/688/whats-the-algorithm-behind-photoshops-black-and-white-adjustment-layer
 */ 
void color_balance(layer_t layer, float reds, float yellows, float greens, float cyans, float blues, float magentas, rect_t zone) {
  reds = remap(reds);
  yellows = remap(yellows);
  greens = remap(greens);
  cyans = remap(cyans);
  blues = remap(blues);
  magentas = remap(magentas);

  printf("reds=%f\nyellows=%f\n,greens=%f\ncyans=%f\nblues=%f\nmagentas=%f\n", reds, yellows, greens, cyans, blues, magentas);
  printf("redsH=%f\nyellowsH=%f\n,greensH=%f\ncyansH=%f\nbluesH=%f\nmagentasH=%f\n", hue_red(), hue_yellow(), hue_green(), hue_cyan(), hue_blue(), hue_magenta());

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
       float currHue = 1.0f * HSL.x;

       float Coef = 0.0f;
       //printf("Hue=%f CurrHue=%f\n", HSL.x, currHue);
       Coef += reds * (currHue - hue_red());
       Coef += yellows * (currHue - hue_yellow());
       Coef += greens * (currHue - hue_green());
       Coef += cyans * (currHue - hue_cyan());
       Coef += blues * (currHue - hue_blue());
       Coef += magentas * (currHue - hue_magenta());
       

       HSL.x -= Coef;
       

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

static float remap(float color)
{
   float base_interval = ADJUST_COLOR_MAX - ADJUST_COLOR_MIN;

   if (color < ADJUST_COLOR_MIN) color = ADJUST_COLOR_MIN;
   if (color > ADJUST_COLOR_MAX) color = ADJUST_COLOR_MAX;

   float k = (color - ADJUST_COLOR_MIN) / base_interval;

   return mix(TGT_BASE_MIN, TGT_BASE_MAX, k);
}

static float hue_red(void) {
   vec3 v = RGBtoHSL(vec3_init(1.0f, 0.0f, 0.0f));
   printf("v.y=%f\n", v.y);
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
