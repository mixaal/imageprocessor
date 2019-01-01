#ifndef __IM_PERMGEN_H__
#define __IM_PERMGEN_H__


typedef struct {
  _Bool first_ready;
  int *array;
  int array_length;
  
} permgen_t;


permgen_t permgen_init(int len);
_Bool permgen_has_more(permgen_t *p);
int *permgen_get_next(permgen_t *p) ;

#endif /* __IM_PERMGEN_H__ */
