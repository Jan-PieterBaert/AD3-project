#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  command *retval = NULL;
  while (retval == NULL) {
    retval = malloc(sizeof(command));
  }
  char c = getchar();
  retval->type = charToCommandType(c);
  scanf("%19s", retval->timestamp);
  if (retval->type == addElement) {
    int currentSize = COMMAND_VALUE_STEP;
    int index = 0;
    char *value = NULL;
    while (value == NULL) {
      value = malloc(currentSize * sizeof(char));
    }
    int ch = getchar();
    while (ch != EOF && ((char)ch != '\n')) {
      value[index] = ch;
      ch = fgetc(stdin);
      index++;
      if (index + 1 == currentSize) {
        currentSize += COMMAND_VALUE_STEP;
        value = realloc(value, currentSize * sizeof(char));
      }
    }
    value[index] = '\0';
    retval->value = value;
  } else {
    retval->value = NULL;
  }
  return retval;
}

void freeCommand(command *c) {
  if (c->value != NULL) {
    free(c->value);
  }
  free(c);
}
