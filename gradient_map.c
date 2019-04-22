#include "gradient_map.h"
#include "common.h"
#include "color_conversion.h"
#include <math.h>

/**
 * Gradient fill.
 *
 * @param layer       layer to fill-in
 * @param start_color start color
 * @param end_color   end color
 * @param weight      where is the center on the vector line (0.5 center, 0.75 shifted till the end)
 * @param opacity     translucency for merge
 * @param start_point normalized coordinates, e.g. (1,1) is the right-bottom corner
 * @param direction   vector from start_point determines the end of fill, e.g. -0.5, 0 means horizontal fill, half of screen from the start_point
 * @param zone        zone to fill
 * @param alpha_mask  values with lower intensity are overwritten
 * @param mask        if true use layer mask, otherwise image pixels are used.
 */
void gradient_fill(
  layer_t layer,
  vec3 start_color,
  vec3 end_color,
  float weight,
  float opacity,
  vec3 start_point,
  vec3 direction,
  rect_t zone,
  float alpha_mask,
  _Bool mask)
{
  color_t *image = (mask) ? layer.mask : layer.image;
  int width = layer.width;
  int height = layer.height;
  int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;

  float cx = zone.minx + (zone.maxx - zone.minx) * start_point.x;
  float cy = zone.miny + (zone.maxy - zone.miny) * start_point.y;
  float vx = (zone.maxx - zone.minx) * direction.x;
  float vy = (zone.maxy - zone.miny) * direction.y;

  float d = sqrtf(vx*vx + vy*vy);
  if (d<0.001f) d = 1.0f;
  float ex = vx / d;
  float ey = vy / d;


  printf("cx=%f cy=%f vx=%f vy=%f ex=%f ey=%f\n", cx, cy, vx, vy, ex, ey);
  weight = saturatef(weight);
  if (weight == 0.0f) weight = 0.5f;
  vec3 center_color = vec3_mix(start_color, end_color, 0.5f);


#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       float Px = x - cx;
       float Py = y - cy;
       float projP_to_v_scalar =  Px * ex + Py * ey;
       float Px_v = ex * projP_to_v_scalar;
       float Py_v = ey * projP_to_v_scalar;

       //if(y==100) {
       //    printf("P=[%f %f; %f], P_v=[%f %f]\n", Px, Py, projP_to_v_scalar, Px_v, Py_v);
       //}
   
       float vc = vx;
       float t;
       if (fabs(vc)<0.001f) {
          vc = vy;
          if (fabs(vc)<0.001f) vc = 1.0f;
          t = Py_v  / vc;
       } else t = Px_v / vc;
       //if(y==100) printf("t=%f vc=%f Pxv-cx=%f Pyv-cy=%f Pxv=%f Pyv=%f", t, vc, Px_v - cx, Py_v-cy, Px_v, Py_v);
       
       if(t<0.0f) t = 0.0f;
       if(t>1.0f) t = 1.0f;
       int idx = y*width*color_components + x*color_components;
       float r, g, b;
       r = (float)image[idx] / COLOR_MAX;
       g = (float)image[idx+1] / COLOR_MAX;
       b = (float)image[idx+2] / COLOR_MAX;

       vec3 newRGB;
       if (t < weight) {
         float k = t / weight;
         newRGB = 
            vec3_add(
              vec3_multiply(vec3_mix(start_color, center_color, k), opacity) ,
              vec3_multiply(vec3_init(r, g, b), 1 - opacity)
            );
       } else {
         float k = (t - weight) / (1-weight);
         newRGB = 
            vec3_add(
              vec3_multiply(vec3_mix(center_color, end_color, k), opacity),
              vec3_multiply(vec3_init(r, g, b), 1 - opacity)
            );
       }
       float Iv = to_gray(newRGB);
       if(Iv < alpha_mask) continue; // do not overwrite lower intensities
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

void gradient_map(
  layer_t layer, 
  vec3 start_color,
  vec3 end_color,
  float weight,
  float opacity,
  rect_t zone) {

  color_t *image = layer.image;
  int width = layer.width;
  int height = layer.height;
  int color_components = layer.color_components;
  if (zone.maxy==0) return;

  if (zone.minx<0) zone.minx=0;
  if (zone.miny<0) zone.miny=0;
  if (zone.maxx>=width) zone.maxx=width;
  if (zone.maxy>=height) zone.maxy=height;
  weight = saturatef(weight);
  if (weight == 0.0f) weight = 0.5f;
  vec3 center_color = vec3_mix(start_color, end_color, 0.5f);

#pragma omp parallel for
  for(int y=zone.miny; y<zone.maxy; y++)  {
    for(int x=zone.minx; x<zone.maxx; x++) {
       int idx = y*width*color_components + x*color_components;
       float r, g, b;
       r = (float)image[idx] / COLOR_MAX;
       g = (float)image[idx+1] / COLOR_MAX;
       b = (float)image[idx+2] / COLOR_MAX;

       vec3 newRGB;
       float Iv = to_gray(vec3_init(r, g, b));
       if (Iv < weight) {
         float k = Iv / weight;
         newRGB = 
            vec3_add(
              vec3_multiply(vec3_mix(start_color, center_color, k), opacity) ,
              vec3_multiply(vec3_init(r, g, b), 1 - opacity)
            );
       } else {
         float k = (Iv - weight) / (1-weight);
         newRGB = 
            vec3_add(
              vec3_multiply(vec3_mix(center_color, end_color, k), opacity),
              vec3_multiply(vec3_init(r, g, b), 1 - opacity)
            );
       }

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
 
