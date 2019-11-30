#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commandParser.h"
#include "misc.h"

commandType charToCommandType(char c) {
  switch (c) {
  case '+':
    return commandAddElement;
  case '-':
    return commandDeleteElement;
  case '?':
    return commandQueryElement;
  case '#':
    return commandQueryRange;
  default:
    return unknownCommand;
  }
}

command *parseCommand() {
  command *retval = (command *)nullSafeMalloc(sizeof(command));
  char c = getc(stdin);
  retval->type = charToCommandType(c);
  for (int i = 0; i < TIMESTAMP_SIZE; i++)
    retval->timestamp[i] = getc(stdin);
  if (retval->type == commandAddElement) {
    c = getc(stdin);
    if ((char)c != ' ')
      retval->type = unknownCommand;
    else {
      int currentSize = COMMAND_VALUE_START;
      int index = 0;
      char *value = nullSafeMalloc(currentSize * sizeof(char));
      int ch = getc(stdin);
      while (ch != EOF && ((char)ch != '\n')) {
        value[index] = ch;
        ch = getc(stdin);
        index++;
        if (index + 1 == currentSize) {
          currentSize *= COMMAND_VALUE_MULTIPLY_STEP;
          value = nullSafeRealloc(value, currentSize * sizeof(char));
        }
      }
      value[index] = '\0';
      retval->value = value;
    }
  } else if (retval->type == commandQueryRange) {
    if ((char)getc(stdin) != ' ')
      retval->type = unknownCommand;

    retval->value = nullSafeMalloc(sizeof(char) * TIMESTAMP_SIZE);
    for (int i = 0; i < TIMESTAMP_SIZE - 1; i++)
      retval->value[i] = getc(stdin);
    retval->value[TIMESTAMP_SIZE - 1] = '\0';
    char ch = getc(stdin);
    while (ch != EOF && ((char)ch != '\n')) {
      ch = getc(stdin);
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
  if (c->value != NULL) {
    free(c->value);
  }
  free(c);
}
