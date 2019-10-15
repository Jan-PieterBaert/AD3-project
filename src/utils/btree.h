#ifndef BTREE
#define BTREE

#include "globs.h"

#define BTREE_PATH_SIZE 5

typedef struct btreeElement {
  /* A value of NULL means it's a tombstone */
  char *value;
  char key[TIMESTAMP_SIZE];
} btreeElement;

typedef struct btree {
  short numberOfKeys;
  btreeElement *elements[NUMBER_OF_BTREE_KEYS];
  struct btree *children[NUMBER_OF_BTREE_KEYS + 1];
} btree;

btreeElement *allocateBtreeElement();
int compareBtreeElements(btreeElement *e1, btreeElement *e2);
void freeBtreeElement(btreeElement *tree);
btree *allocateBtree();
btree *addToBtree(btree *root, btreeElement *element);
btreeElement *addElementToBtreeLeaf(btree **tree, btreeElement *element,
                                    int index);
btreeElement *addElementToBtreeChild(btree **tree, btreeElement *element,
                                     int index);
btreeElement *searchElement(btree *tree, btreeElement *element, int**path);
void freeBtree(btree *tree);

#endif
