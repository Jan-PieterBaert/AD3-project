#include "utils/globs.h"
#include "utils/btree.h"
#include "utils/commandParser.h"
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
#ifdef DEBUG
    printf("Timestamp: %s | Command: %d | Value: %s\n", c->timestamp,
           (int)c->type, c->value);
#endif
    if (c->type == addElement) {
      btreeElement *element = allocateBtreeElement();
      strncpy(element->key, c->timestamp, TIMESTAMP_SIZE);
      element->value = c->value;
      tree = addToBtree(tree, element);
    } else if (c->type == deleteElement) {
    } else if (c->type == queryElement) {
      btreeElement *element = allocateBtreeElement();
      strncpy(element->key, c->timestamp, TIMESTAMP_SIZE);
      btreeElement *result = searchElement(tree, element, NULL);
      if (result != NULL && result->value != NULL)
        printf("!%s\n", result->value);
      else
        printf("?\n");
      freeBtreeElement(element);
    } else if (c->type == queryRange) {
    } else {
      printf("Unable to parse commands correctly\n");
      return 1;
    }
#ifdef DEBUG
    printf("Number of keys: %d\n", tree->numberOfKeys);
    printf("\n");
#endif
    freeCommand(c);
    ch = getc(stdin);
  }
  freeBtree(tree);
  return 0;
}
