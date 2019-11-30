#include "utils/btree.h"
#include "utils/commandParser.h"
#include "utils/globs.h"
#include "utils/misc.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
#ifdef DEBUG
  printf("Starting the `history` program\n");
#endif
  int ch = getc(stdin);
  // As long as we are not at the end of the input, try to parse a command and
  // execute it
  btree *tree = allocateBtree();
  while (ch != EOF) {
    ungetc(ch, stdin);
    command *c = parseCommand();

    if (c->type == commandAddElement) {
      btreeElement element;
      strncpy(element.key, c->timestamp, TIMESTAMP_SIZE);
      int valueLength = strlen(c->value) + 1;
      element.value = nullSafeMalloc(valueLength);
      strncpy(element.value, c->value, valueLength);
      tempBtree *tempTree = allocateTempBtree();
      tempTree->element = element;
      tree = addToBtree(tree, tempTree);
      freeTempBtree(tempTree);

    } else if (c->type == commandDeleteElement) {
      btreeElement element;
      strncpy(element.key, c->timestamp, TIMESTAMP_SIZE);
      deleteElement(tree, element);

    } else if (c->type == commandQueryElement) {
      btreeElement element;
      strncpy(element.key, c->timestamp, TIMESTAMP_SIZE);
      int searchIndex;
      bool found = false;
      btree *result = searchElement(tree, element, &searchIndex, &found);
      if (found && result->elements[searchIndex].value != NULL)
        printf("!%s\n", result->elements[searchIndex].value);
      else
        printf("?\n");

    } else if (c->type == commandQueryRange) {
      int numberInQuery;
      c->timestamp[TIMESTAMP_SIZE - 1] = '\0';
      c->value[TIMESTAMP_SIZE - 1] = '\0';
      btreeElement element1, element2;
      strncpy(element1.key, c->timestamp, TIMESTAMP_SIZE);
      strncpy(element2.key, c->value, TIMESTAMP_SIZE);

      if (strcmp(c->timestamp, c->value) <= 0)
        numberInQuery = rangeQuery(tree, element1, element2);
      else
        numberInQuery = rangeQuery(tree, element2, element1);
      printf("%d\n", numberInQuery);
    } else
      printErrorAndExit(UNKNOWN_COMMAND_EXIT_CODE,
                        "Unable to parse commands correctly");

    freeCommand(c);
    ch = getc(stdin);
  }
  freeBtree(tree);
  return 0;
}
