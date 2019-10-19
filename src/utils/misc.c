#include <stdio.h>
#include <stdlib.h>

#include "globs.h"
#include "misc.h"

void *nullSafeMalloc(size_t size) {
  void *retval = NULL;
  retval = malloc(size);
  if (retval == NULL)
    printErrorAndExit(OUT_OF_MEMORY_EXIT_CODE, "Unable to allocate memory");
  return retval;
}

void *nullSafeRealloc(void *originalPointer, size_t size) {
  void *retval = realloc(originalPointer, size);
  if (retval == NULL)
    printErrorAndExit(OUT_OF_MEMORY_EXIT_CODE, "Unable to reallocate memory");
  return retval;
}

void printErrorAndExit(int exitCode, char errorString[]) {
  fprintf(stdout, "Error: %s\n", errorString);
  exit(exitCode);
}
