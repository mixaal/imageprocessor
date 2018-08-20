#ifndef __IM_COMMON_TYPES_H__
#define __IM_COMMON_TYPES_H__ 1

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define False 0
#define True 1

typedef unsigned char color_t;
typedef struct { float x, y, z; }  vec3;
typedef struct { float x, y, z, w; } vec4;

#define COLOR_MAX 255
#define COLOR_MID 128

#endif /* __IM_COMMON_TYPES_H__ */
