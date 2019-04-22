#ifndef __IM_AUTO_TONE_H__
#define __IM_AUTO_TONE_H__ 1


#include "layer.h"
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void auto_tone(image_t layer, rect_t zone);
void hdr(image_t layer, rect_t zone);

#ifdef __cplusplus
}
#endif

#endif /* __IM_AUTO_TONE_H__ */
