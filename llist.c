#include <stdio.h>
#include "llist.h"
#include "xmalloc.h"

static void allocate_next_chunk(node_t *start);

node_t *llist_init() {
   node_t *start = xmalloc(sizeof(node_t));
   start->size = 0;
   start->next = NULL;
   start->current = start;
   return start;
}

void llist_add(node_t *start, vec3 element) {
   if (start -> current -> size >= ELEMENTS_SZ )  {
       allocate_next_chunk(start);
   }
   start -> current -> array[start -> current -> size] = element;
   start -> current -> size ++;
}

void llist_walk(node_t *start, walkfunc_t walkfunc)
{
   node_t *current = start;
   while(current != NULL) {
     for(int i=0; i<current -> size; i++) {
        walkfunc(current->array[i]);
     }
     current = current->next;
   }
}

static void allocate_next_chunk(node_t *start) {
   node_t *next = llist_init();
   start -> current -> next = next;
   start -> current = next;
}
