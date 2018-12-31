#ifndef __IM_LLIST_H__
#define __IM_LLIST_H__

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ELEMENTS_SZ 1024

typedef struct node {
  vec3 array[ELEMENTS_SZ];
  size_t size;
  struct node *next;
  struct node *current;
} node_t; 

typedef void (*walkfunc_t)(vec3 element);


node_t *llist_init();
void llist_add(node_t *start, vec3 element);
void llist_walk(node_t *start, walkfunc_t walkfunc);


#ifdef __cplusplus
}
#endif
#endif /* __IM_LLIST_H__ */
