#ifndef MISC_FUNCTIONS
#define MISC_FUNCTIONS

#include <stdlib.h>

void *nullSafeMalloc(size_t size);
void *nullSafeRealloc(void *originalPointer, size_t size);
void printErrorAndExit(int exitCode, char errorString[]);

#endif
