#include <stdlib.h>

#include "misc.h"
#include "globs.h"

void *nullSafeMalloc(size_t size) {
  void *retval = NULL;
  int i = 0;
  while (retval == NULL && i < MAX_MALLOC_TRIES) {
    retval = malloc(size);
    i++;
  }
  return retval;
}
