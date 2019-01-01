/**
 * https://rosettacode.org/wiki/Permutations#Java
 */
#include <stdio.h>
#include "permgen.h"
#include "xmalloc.h"

permgen_t permgen_init(int len)
{
    permgen_t r;

    r.first_ready = 0;
    r.array = xmalloc(sizeof(int)*len);
    r.array_length = len;

    for(int i=0; i<len; i++) r.array[i] = i;

    return r;
}

_Bool permgen_has_more(permgen_t *p)
{
  _Bool end = p->first_ready;
  for(int i=1; i < p->array_length; i++) {
    end = end && (p->array[i] < p->array[i-1]);
  }
  return !end;
}

int *permgen_get_next(permgen_t *p) 
{
  if (!p->first_ready) {
     p->first_ready = 1;
     return p->array;
  }
  
  int temp;
  int j = p->array_length - 2;
  int k = p->array_length - 1;

  for (;p->array[j] > p->array[j+1]; j--);
  for (;p->array[j] > p->array[k]; k--);

  temp = p->array[k];
  p->array[k] = p->array[j];
  p->array[j] = temp;

  int r = p->array_length - 1;
  int s = j + 1;
 
   while (r > s) {
      temp = p->array[s];
      p->array[s++] = p->array[r];
      p->array[r--] = temp;
   }
 
   return p->array;
}

#ifdef TEST_MAIN

static void print_perm(int *p, int l)
{
   for(int i=0; i<l; i++) printf("%d ", p[i]);
   printf("\n");
}

int main(void) {
    permgen_t p = permgen_init(9);
    while(permgen_has_more(&p)) {
       int *perm = permgen_get_next(&p);
       print_perm(perm, 9);
    }
   return 0;
}
#endif
