#include "btree.h"
#include <stdio.h>
#include <string.h>

#include "misc.h"

#ifdef DEBUG
/* Print one Btree element */
void printBtreeElement(btreeElement element, int i) {
  char *value = "Empty";
  if (element.value)
    value = element.value;
  printf("Element %d with key %.*s and value %s\n", i, TIMESTAMP_SIZE,
         element.key, value);
}

/* Print a Btree recursively */
void printBtree(btree *tree, int depth) {
  if (tree) {
    printf("BTree on depth %d with %d keys\n", depth, tree->numberOfKeys);
    for (int i = 0; i < tree->numberOfKeys; i++)
      printBtreeElement(tree->elements[i], i);
    for (int i = 0; i < tree->numberOfKeys + 1 && tree->children[i] != NULL;
         i++) {
      printf("\n");
      printf("Printing child %d\n", i);
      printBtree(tree->children[i], depth + 1);
    }
  } else
    printf("Tree is NULL at depth %d\n", depth);
}
#endif

/* Allocate space for one Btree and initialize it */
btree *allocateBtree() {
  btree *retval = nullSafeMalloc(sizeof(btree));
  retval->children[0] = NULL;
  retval->parent = NULL;
  retval->numberOfKeys = 0;
  return retval;
}

/* Allocate space for one tempBtree and initialize it */
tempBtree *allocateTempBtree() {
  tempBtree *retval = nullSafeMalloc(sizeof(tempBtree));
  retval->children[0] = NULL;
  retval->children[1] = NULL;
  return retval;
}

/* Free the space occipied by a Btree and its children */
void freeBtree(btree *tree) {
  if (tree == NULL)
    return;
  for (int i = 0; i < tree->numberOfKeys; i++)
    free(tree->elements[i].value);
  if (tree->children[0])
    for (int i = 0; i < tree->numberOfKeys + 1; i++)
      freeBtree(tree->children[i]);
  free(tree);
}

/* Free the space occipied by a tempBtree */
void freeTempBtree(tempBtree *tree) { free(tree); }

void changeBtreeElementValue(btreeElement *element, char *newValue) {
  free(element->value);
  element->value = newValue;
}

/* Insert a temp Btree in a Btree and return a temp Btree if it is needed */
tempBtree *insertTempBtreeInBtree(btree *tree, tempBtree *tempTree,
                                  int *index) {
  int i = *index;
  if (tree->numberOfKeys < NUMBER_OF_BTREE_KEYS) {
    for (int j = tree->numberOfKeys; j > i; j--)
      tree->elements[j] = tree->elements[j - 1];
    tree->elements[i] = tempTree->element;
    if (tree->children[0]) {
      for (int j = tree->numberOfKeys + 1; j > i + 1; j--)
        tree->children[j] = tree->children[j - 1];
      free(tree->children[i]);
      tree->children[i] = tempTree->children[0];
      tree->children[i + 1] = tempTree->children[1];
      tree->children[i]->parent = tree;
      tree->children[i + 1]->parent = tree;
    }
    tree->numberOfKeys++;
  } else {
    tempBtree *retval = allocateTempBtree();
    /* Initialize the left and right subtrees */
    btree *leftTree = allocateBtree();
    leftTree->numberOfKeys = NUMBER_OF_BTREE_KEYS / 2;
    btree *rightTree = allocateBtree();
    rightTree->numberOfKeys = NUMBER_OF_BTREE_KEYS / 2;

    /* If there are children, put them all in a list with the new children in
     * the right positions*/
    if (tempTree->children[0]) {
      btree *children[NUMBER_OF_BTREE_KEYS + 2];
      for (int j = 0; j < i; j++)
        children[j] = tree->children[j];
      children[i] = tempTree->children[0];
      children[i + 1] = tempTree->children[1];
      for (int j = i + 1; j < NUMBER_OF_BTREE_KEYS + 1; j++)
        children[j + 1] = tree->children[j];
      for (int j = 0; j < NUMBER_OF_BTREE_KEYS / 2 + 1; j++) {
        leftTree->children[j] = children[j];
        leftTree->children[j]->parent = leftTree;
        rightTree->children[j] = children[j + NUMBER_OF_BTREE_KEYS / 2 + 1];
        rightTree->children[j]->parent = rightTree;
      }
      /* Otherwise the children of the new tempBtree are both NULL */
    }

    /* The middle tempTree is the tempTree which needs to go up*/
    /* retval->element = allElements[NUMBER_OF_BTREE_KEYS / 2]; */

#ifdef DEBUG
    printf("i:%d\n", i);
#endif

    /* The other elements can be copied over to the left/right-subtrees*/
    if (i < NUMBER_OF_BTREE_KEYS / 2) {
      for (int j = 0; j < NUMBER_OF_BTREE_KEYS / 2; j++)
        rightTree->elements[j] = tree->elements[j + NUMBER_OF_BTREE_KEYS / 2];

      for (int j = 0; j < i; j++)
        leftTree->elements[j] = tree->elements[j];
      leftTree->elements[i] = tempTree->element;
      for (int j = i + 1; j < NUMBER_OF_BTREE_KEYS / 2; j++)
        leftTree->elements[j] = tree->elements[j - 1];
      retval->element = tree->elements[NUMBER_OF_BTREE_KEYS / 2 - 1];

    } else if (i > NUMBER_OF_BTREE_KEYS / 2) {
      for (int j = 0; j < NUMBER_OF_BTREE_KEYS / 2; j++)
        leftTree->elements[j] = tree->elements[j];

      i -= NUMBER_OF_BTREE_KEYS / 2;
      --i;
      for (int j = 0; j < i; j++)
        rightTree->elements[j] =
            tree->elements[j + NUMBER_OF_BTREE_KEYS / 2 + 1];
      rightTree->elements[i] = tempTree->element;
      for (int j = i + 1; j < NUMBER_OF_BTREE_KEYS / 2; j++)
        rightTree->elements[j] = tree->elements[j + NUMBER_OF_BTREE_KEYS / 2];

      retval->element = tree->elements[NUMBER_OF_BTREE_KEYS / 2];
    } else {
      retval->element = tempTree->element;
      for (int j = 0; j < NUMBER_OF_BTREE_KEYS / 2; j++) {
        leftTree->elements[j] = tree->elements[j];
        rightTree->elements[j] = tree->elements[j + NUMBER_OF_BTREE_KEYS / 2];
      }
    }
#ifdef DEBUG
    printBtree(leftTree, 100);
    printBtree(rightTree, 200);
    printf("To up element\n");
    printBtreeElement(retval->element, 0);
    printf("\n");
#endif

    retval->children[0] = leftTree;
    retval->children[1] = rightTree;
    return retval;
  }
  return NULL;
}

/* Add a temp Btree to the Btree */
btree *addToBtree(btree *root, tempBtree *tempTree) {
  int treeIndex = 0;
  bool found = false;
  btree *tree = searchElement(root, tempTree->element, &treeIndex, &found);
  if (found) {
    /* If the element is already in the tree, change the value*/
    changeBtreeElementValue(&tree->elements[treeIndex],
                            tempTree->element.value);
  } else {
    /* The index where we need to insert is the index found by searching*/
    int i = treeIndex;

    tempBtree *temp = tempTree;
    searchElementOnDepth(tree, temp->element, &i, &found, 1);
    temp = insertTempBtreeInBtree(tree, tempTree, &i);
    while (temp && tree->parent) {
      tree = tree->parent;
      i = 0;
      found = false;
      searchElementOnDepth(tree, temp->element, &i, &found, 1);
      tempBtree *nextTemp = insertTempBtreeInBtree(tree, temp, &i);
      freeTempBtree(temp);
      temp = nextTemp;
    }

    /* Change the root if it's needed*/
    if (temp && tree->parent == NULL) {
      root->children[0] = NULL;
      root->numberOfKeys = 0;
      freeBtree(root);
      btree *newRoot = allocateBtree();
      newRoot->elements[0] = temp->element;
      newRoot->children[0] = temp->children[0];
      newRoot->children[1] = temp->children[1];
      newRoot->numberOfKeys = 1;
      newRoot->children[0]->parent = newRoot;
      newRoot->children[1]->parent = newRoot;
      root = newRoot;
    }
    freeTempBtree(temp);
  }
  return root;
}

/* Search an element in the Btree */
btree *searchElement(btree *tree, btreeElement element, int *treeIndex,
                     bool *found) {
  return searchElementOnDepth(tree, element, treeIndex, found, -1);
}

/* Search an element in the Btree with a maximum depth level */
btree *searchElementOnDepth(btree *tree, btreeElement element, int *treeIndex,
                            bool *found, int maxDepth) {
  *treeIndex = 0;
  if (tree->numberOfKeys == 0)
    return tree;
  int middle = 0;
  bool isLeaf = false;
  int currentDepth = 0;
  while (!*found && !isLeaf && (maxDepth > currentDepth || maxDepth == -1)) {
    currentDepth++;
    int lowerBound = 0;
    int higherBound = tree->numberOfKeys - 1;

    while (lowerBound < higherBound - 1) {
      middle = lowerBound + (higherBound - lowerBound) / 2;
      if (strcmp(tree->elements[middle].key, element.key) < 0)
        lowerBound = middle;
      else
        higherBound = middle;
    }
    middle = lowerBound;

    while (middle < higherBound &&
           strcmp(tree->elements[middle].key, element.key) < 0)
      middle++;
    short cmpvalue = strcmp(tree->elements[middle].key, element.key);
    if (cmpvalue == 0)
      *found = true;
    else {
      if (cmpvalue < 0)
        middle++;
      btree *newTree = tree->children[middle];
      if (tree->children[0] == NULL)
        /* When the tree is actually a leaf stop searching */
        isLeaf = true;
      else
        tree = newTree;
    }
  }
  *treeIndex = middle;
  return tree;
}

/* Delete an element in the Btree */
void deleteElement(btree *root, btreeElement element) {
  int treeIndex = 0;
  bool found = false;
  btree *tree = searchElement(root, element, &treeIndex, &found);
  if (found && tree->elements[treeIndex].value) {
    /* Make the element a tombstone */
    changeBtreeElementValue(&tree->elements[treeIndex], NULL);
    printf("-\n");
  } else
    printf("?\n");
}

/* Count the number of elements between lowerBound and upperBound */
int rangeQuery(btree *tree, btreeElement lowerBound, btreeElement upperBound) {
  if (tree == NULL || tree->numberOfKeys == 0)
    return 0;

  int lowerIndex = 0;
  bool lowerFound = false;
  searchElementOnDepth(tree, lowerBound, &lowerIndex, &lowerFound, 1);

  int upperIndex = 0;
  bool upperFound = false;
  searchElementOnDepth(tree, upperBound, &upperIndex, &upperFound, 1);

  int retval = 0;
  for (int i = lowerIndex; i < upperIndex; i++)
    if (tree->elements[i].value)
      ++retval;

  if (tree->children[0])
    for (int i = lowerIndex; i < upperIndex + 1; ++i)
      retval += rangeQuery(tree->children[i], lowerBound, upperBound);

  return retval;
}
