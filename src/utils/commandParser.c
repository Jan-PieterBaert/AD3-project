#include <stdio.h>
#include <stdlib.h>

#include "commandParser.h"

commandType charToCommandType(char c) {
  switch (c) {
  case '+':
    return addElement;
  case '-':
    return deleteElement;
  case '?':
    return queryElement;
  case '#':
    return queryRange;
  default:
    return unknownCommand;
  }
}

command *parseCommand() {
  char c;
  scanf("%c", &c);
  commandType type = charToCommandType(c);
  printf("%d\n", (int)type);
  command *retval = malloc(sizeof(command));
  retval->type = type;
  retval->timestamp = NULL;
  return retval;
}

void freeCommand(command* c){
    /* free(c->timestamp); */
    free(c);
}
