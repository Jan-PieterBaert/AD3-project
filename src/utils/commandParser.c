#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commandParser.h"
#include "misc.h"

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
  command *retval = (command *)nullSafeMalloc(sizeof(command));
  char c = getc(stdin);
  retval->type = charToCommandType(c);
  int i = 0;
  while (i < TIMESTAMP_SIZE) {
    retval->timestamp[i] = getc(stdin);
    i++;
  }
  /* retval->timestamp[TIMESTAMP_SIZE - 1] = '\0'; */
  /* regexMatch op [0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2} */
  if (retval->type == addElement) {
    c = getc(stdin);
    if ((char)c != ' ')
      retval->type = unknownCommand;
    else {
      int currentSize = COMMAND_VALUE_STEP;
      int index = 0;
      char *value = nullSafeMalloc(currentSize * sizeof(char));
      int ch = getc(stdin);
      while (ch != EOF && ((char)ch != '\n')) {
        value[index] = ch;
        ch = getc(stdin);
        index++;
        if (index + 1 == currentSize) {
          currentSize += COMMAND_VALUE_STEP;
          value = realloc(value, currentSize * sizeof(char));
        }
      }
      value[index] = '\0';
      retval->value = value;
    }
  } else {
    int ch = getc(stdin);
    while (ch != EOF && ((char)ch != '\n')) {
      ch = getc(stdin);
    }
    retval->value = NULL;
  }
  return retval;
}

void freeCommand(command *c) {
  /* if (c->value != NULL) { */
  /*   free(c->value); */
  /* } */
  free(c);
}
