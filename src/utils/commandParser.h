#ifndef COMMANDPARSER
#define COMMANDPARSER

#include "globs.h"

#define COMMAND_VALUE_START 25
#define COMMAND_VALUE_MULTIPLY_STEP 5

typedef enum commandType {
  commandAddElement,
  commandDeleteElement,
  commandQueryElement,
  commandQueryRange,
  unknownCommand
} commandType;
typedef struct command {
  commandType type;
  char timestamp[TIMESTAMP_SIZE];
  char* value;
} command;

command *parseCommand();
void freeCommand(command* c);

#endif
