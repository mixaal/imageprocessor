#include <stdio.h>
#include <stdlib.h>

#include "xmalloc.h"

void *xmalloc(size_t nbytes) {
  void *r = malloc(nbytes);
  if (r == NULL) {
     fprintf(stderr, "Not enough memory.\n");
     exit(EXIT_FAILURE);
  }
  return r;
}
