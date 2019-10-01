#include <stdio.h>

#include "utils/commandParser.h"

int main(int argc, char *argv[]) {
  printf("Starting the `history` program\n");
  command *c = parseCommand();
  freeCommand(c);
  return 0;
}
