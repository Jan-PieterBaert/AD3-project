#ifndef COMMANDPARSER
#define COMMANDPARSER

#define TIMESTAMP_SIZE 19
#define COMMAND_VALUE_STEP 10

typedef enum commandType {
  addElement,
  deleteElement,
  queryElement,
  queryRange,
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
