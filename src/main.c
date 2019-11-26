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

    /* #ifdef DEBUG */
    /*     printf("Timestamp: %s | Command: %d | Value: %s\n", c->timestamp, */
    /*            (int)c->type, c->value); */
    /* #endif */

    if (c->type == commandAddElement) {
      btreeElement *element = allocateBtreeElement();
      strncpy(element->key, c->timestamp, TIMESTAMP_SIZE);
      int valueLength = strlen(c->value) + 1;
      element->value = nullSafeMalloc(valueLength);
      strncpy(element->value, c->value, valueLength);
      tempBtree *tempTree = allocateTempBtree();
      tempTree->element = element;
      tree = addToBtree(tree, tempTree);
      freeTempBtree(tempTree);

    } else if (c->type == commandDeleteElement) {
      btreeElement *element = allocateBtreeElement();
      strncpy(element->key, c->timestamp, TIMESTAMP_SIZE);
      deleteElement(tree, element);
      freeBtreeElement(element);

    } else if (c->type == commandQueryElement) {
      btreeElement *element = allocateBtreeElement();
      strncpy(element->key, c->timestamp, TIMESTAMP_SIZE);
      int searchIndex;
      bool found = false;
      btree *result = searchElement(tree, element, &searchIndex, &found);
      if (found && result->elements[searchIndex]->value != NULL)
        printf("!%s\n", result->elements[searchIndex]->value);
      else
        printf("?\n");
      freeBtreeElement(element);

    } else if (c->type == commandQueryRange) {
      int numberInQuery;
      if (strcmp(c->timestamp, c->value)<=0)
        numberInQuery = rangeQuery(tree, c->timestamp, c->value);
      else
        numberInQuery = rangeQuery(tree, c->value, c->timestamp);
      printf("%d\n", numberInQuery);
    } else
      printErrorAndExit(UNKNOWN_COMMAND_EXIT_CODE,
                        "Unable to parse commands correctly");

#ifdef DEBUG
    printf("Number of keys in root: %d\n", tree->numberOfKeys);
    printf("Root elements:\n");
    for (int i = 0; i < tree->numberOfKeys; i++)
      printf("-- key: %s | value: %s\n", tree->elements[i]->key,
             tree->elements[i]->value);
    printf("\n");
#endif
    freeCommand(c);
    ch = getc(stdin);
  }
#ifdef DEBUG
  printBtree(tree, 0);
#endif
  freeBtree(tree);
  return 0;
}
