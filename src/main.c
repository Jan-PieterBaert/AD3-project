#include <stdio.h>
#include <stdlib.h>

#include "utils/commandParser.h"
/* #define DEBUG */
//TODO: add general function to malloc and catch NULL error in malloc

int main(int argc, char *argv[]) {
#ifdef DEBUG
  printf("Starting the `history` program\n");
#endif
  int ch = getc(stdin);
  // As long as we are not at the end of the input, try to parse a command and execute it
  while (ch != EOF) {
    ungetc(ch, stdin);
    command *c = parseCommand();
#ifdef DEBUG
    printf("Timestamp: %s | Command: %d | Value: %s\n", c->timestamp,
           (int)c->type, c->value);
#endif
    freeCommand(c);
    ch = getc(stdin);
  }
  return 0;
}
