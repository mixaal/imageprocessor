#include "common.h"
#include "color_conversion.h"
#include "color_balance.h"

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

/** 
 * Adjust color saturation. Range for color saturation: [0, 1]
 */ 
void adjust_color_saturation(layer_t layer, float reds, float yellows, float greens, float cyans, float blues, float magentas, rect_t zone) {
  reds = clamp(reds, -1 , 1);
  yellows = clamp(yellows, -1, 1);
  greens = clamp(greens, -1, 1);
  cyans = clamp(cyans, -1, 1);
  blues = clamp(blues, -1, 1);
  magentas = clamp(magentas, -1, 1);

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

       if (inside(HSL.x, hue_red())) {
         HSL.y += reds;
       }
       
       if (inside(HSL.x, hue_yellow())) {
         HSL.y += yellows;
       }
       if (inside(HSL.x, hue_green())) {
         HSL.y += greens;
       }
       if (inside(HSL.x, hue_cyan())) {
         HSL.y += cyans;
       }
       if (inside(HSL.x, hue_blue())) {
         HSL.y += blues;
       }
       if (inside(HSL.x, hue_magenta())) {
         HSL.y += magentas;
       }

       HSL.y = saturatef(HSL.y);
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
