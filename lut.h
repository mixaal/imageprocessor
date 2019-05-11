#ifndef __IM_LUT_H__
#define __IM_LUT_H__ 1

#include <common.h>
#include <layer.h>

typedef struct {
  vec3 domain_max, domain_min; 
  int lut_size;
  vec3 *color_grade;
} lut_t;


_Bool read_lut(const char *filename, lut_t *lut);
void lut_translate(layer_t layer, lut_t lut, rect_t zone);

#endif /* __IM_LUT_H__ */
