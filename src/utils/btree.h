#ifndef BTREE
#define BTREE

#include "globs.h"
#include <stdbool.h>

typedef struct btreeElement {
  /* A value of NULL means it's a tombstone */
  char *value;
  char key[TIMESTAMP_SIZE];
} btreeElement;

typedef struct btree {
  short numberOfKeys;
  struct btree *parent;
  btreeElement *elements[NUMBER_OF_BTREE_KEYS];
  struct btree *children[NUMBER_OF_BTREE_KEYS + 1];
} btree;

/* Struct used for adding an element to a btree */
typedef struct tempBtree {
  struct btreeElement *element;
  struct btree *children[2];
} tempBtree;

void printBtree(btree *tree, int depth);
btreeElement *allocateBtreeElement();
void freeBtreeElement(btreeElement *tree);
btree *allocateBtree();
void freeBtree(btree *tree);
tempBtree *allocateTempBtree();
void freeTempBtree(tempBtree *tree);

int compareBtreeElements(btreeElement *e1, btreeElement *e2);
btree *addToBtree(btree *root, tempBtree *element);
btree *searchElement(btree *tree, btreeElement *element, int *index,
                     bool *found);
btree *searchElementOnDepth(btree *tree, btreeElement *element, int *treeIndex,
                            bool *found, int maxDepth);
void deleteElement(btree *tree, btreeElement *element);
int rangeQuery(btree *tree, btreeElement *lowerBound, btreeElement *upperBound);

#endif
