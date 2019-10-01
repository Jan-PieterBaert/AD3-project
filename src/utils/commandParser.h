#ifndef COMMANDPARSER
#define COMMANDPARSER

typedef enum commandType {
  addElement,
  deleteElement,
  queryElement,
  queryRange,
  unknownCommand
} commandType;
typedef struct command {
  commandType type;
  char *timestamp;
} command;

command *parseCommand();
void freeCommand(command* c);

#endif
