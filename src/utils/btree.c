#include "btree.h"
#include <stdio.h>
#include <string.h>

#include "misc.h"

void printBtree(btree *tree, int depth) {
  printf("BTree on depth %d with %d keys\n", depth, tree->numberOfKeys);
  for (int i = 0; i < tree->numberOfKeys; i++)
    printf("Element %d with key %s and value %s\n", i, tree->elements[i]->key,
           tree->elements[i]->value);
  for (int i = 0; i < tree->numberOfKeys + 1 && tree->children[i] != NULL;
       i++) {
    printf("Printing child %d\n", i);
    printBtree(tree->children[i], depth + 1);
  }
}

btreeElement *allocateBtreeElement() {
  btreeElement *retval = nullSafeMalloc(sizeof(btreeElement));
  retval->value = NULL;
  return retval;
}

btree *allocateBtree() {
  btree *retval = nullSafeMalloc(sizeof(btree));
  retval->children[0] = NULL;
  retval->numberOfKeys = 0;
  return retval;
}

tempBtree *allocateTempBtree() {
  tempBtree *retval = nullSafeMalloc(sizeof(tempBtree));
  retval->children[0] = NULL;
  retval->children[1] = NULL;
  retval->element = NULL;
  return retval;
}

int compareBtreeElements(btreeElement *e1, btreeElement *e2) {
  return strcmp(e1->key, e2->key);
}

void freeBtreeElement(btreeElement *e1) {
  if (e1->value != NULL) {
    free(e1->value);
  }
  free(e1);
}

void freeBtree(btree *tree) {
  if (tree->children[0] != NULL) {
    for (int i = 0; i < tree->numberOfKeys; i++) {
      freeBtreeElement(tree->elements[i]);
      freeBtree(tree->children[i]);
    }
    freeBtree(tree->children[tree->numberOfKeys]);
  }
  free(tree);
}

void freeTempBtree(tempBtree *tree) { free(tree); }

tempBtree *insertTempBtreeInBtree(btree **t, tempBtree *element, int *index) {
  int i = *index;
  btree *tree = *t;
  if (tree->numberOfKeys < NUMBER_OF_BTREE_KEYS) {
    for (int j = tree->numberOfKeys; j > i; j--)
      tree->elements[j] = tree->elements[j - 1];
    tree->elements[i] = element->element;
    if (tree->children[0] != NULL) {
      for (int j = tree->numberOfKeys + 1; j > i + 1; j--)
        tree->children[j] = tree->children[j - 1];
      tree->children[i] = element->children[0];
      tree->children[i + 1] = element->children[1];
      tree->children[i]->parent = tree;
      tree->children[i + 1]->parent = tree;
    }
    tree->numberOfKeys++;
    return NULL;
  } else {
    tempBtree *retval = allocateTempBtree();
    /* Initialize the left and right subtrees */
    btree *leftTree = allocateBtree();
    leftTree->numberOfKeys = NUMBER_OF_BTREE_KEYS / 2;
    btree *rightTree = allocateBtree();
    rightTree->numberOfKeys = NUMBER_OF_BTREE_KEYS / 2;

    /* TODO: Refactor the below if possible*/
    /* Put all elements in a temporal list with the new element on the right
     * position*/
    btreeElement **allElements =
        nullSafeMalloc((NUMBER_OF_BTREE_KEYS + 1) * sizeof(btreeElement *));
    for (int j = 0; j < i; j++)
      allElements[j] = tree->elements[j];
    allElements[i] = element->element;
    for (int j = i + 1; j < NUMBER_OF_BTREE_KEYS + 1; j++)
      allElements[j] = tree->elements[j - 1];

    /* If there are children, put them all in a list with the new children in
     * the right positions*/
    if (element->children[0] != NULL) {
      btree **children =
          nullSafeMalloc((NUMBER_OF_BTREE_KEYS + 2) * sizeof(btree *));
      for (int j = 0; j < i; j++)
        children[j] = tree->children[j];
      children[i] = element->children[0];
      children[i + 1] = element->children[1];
      for (int j = i + 2; j < NUMBER_OF_BTREE_KEYS + 2; j++)
        children[j] = tree->children[j - 2];
      for (int i = 0; i < NUMBER_OF_BTREE_KEYS / 2 + 1; i++) {
        leftTree->children[i] = children[i];
        rightTree->children[i] = children[i + NUMBER_OF_BTREE_KEYS / 2 + 1];
      }
      retval->children[0] = children[NUMBER_OF_BTREE_KEYS / 2];
      retval->children[1] = children[NUMBER_OF_BTREE_KEYS / 2 + 1];
      /* Otherwise the children of the new tempBtree are both NULL*/
    } else {
      retval->children[0] = NULL;
      retval->children[1] = NULL;
    }

    /* The middle element is the element which needs to go up*/
    retval->element = allElements[NUMBER_OF_BTREE_KEYS / 2];

    /* The other elements can be copied over to the left/right-subtrees*/
    for (int i = 0; i < NUMBER_OF_BTREE_KEYS / 2; i++) {
      leftTree->elements[i] = allElements[i];
      rightTree->elements[i] = allElements[i + NUMBER_OF_BTREE_KEYS / 2 + 1];
    }
    retval->children[0] = leftTree;
    retval->children[1] = rightTree;

#ifdef DEBUG
    printf("TO UP KEY: %s\n", retval->element->key);
    for (int k = 0; k < NUMBER_OF_BTREE_KEYS + 1; k++) {
      printf("-- key: %s | value: %s | index:%d\n", allElements[k]->key,
             allElements[k]->value, k);
    }
#endif
    return retval;
  }
}

btree *addToBtree(btree *root, tempBtree *element) {
  /* Find index where to insert, then insert element */
  int treeIndex = 0;
  btree *tree = searchElement(root, element->element, &treeIndex);
  if (treeIndex > -1) {
    free(tree->elements[treeIndex]->value);
    tree->elements[treeIndex]->value = element->element->value;
    element->element->value = NULL;
  } else {
    int i = 0;
    /* Find the index where we need to insert (TODO change this search to binary
     * search)
     */
    while (i < tree->numberOfKeys &&
           compareBtreeElements(tree->elements[i], element->element) < 0) {
      i++;
    }

    /*
     * Insert in the subtree (with function)
     *      Returns tempBtree* when the element got split up, else it will
     * return NULL If the tempBtree* is returned when the tree we have given is
     * the root Make the element of tempBtree the root element Make the
     * left/right Btrees the 2 children
     *  TODO: Fix ^^ needs checking!!
     */
    tempBtree *temp = element;
    temp = insertTempBtreeInBtree(&tree, element, &i);
    while (temp != NULL && tree->parent != NULL) {
      tree = tree->parent;
      i = 0;
      while (i < tree->numberOfKeys &&
             compareBtreeElements(tree->elements[i], temp->element) < 0) {
        i++;
      }
      temp = insertTempBtreeInBtree(&tree, temp, &i);
    }

    /* Change the root */
    if (temp != NULL && tree->parent == NULL) {
      btree *newRoot = allocateBtree();
      newRoot->elements[0] = temp->element;
      newRoot->children[0] = temp->children[0];
      newRoot->children[1] = temp->children[1];
      newRoot->numberOfKeys = 1;
      newRoot->children[0]->parent = newRoot;
      newRoot->children[1]->parent = newRoot;
      return newRoot;
    }
  }
  return root;
}

btree *searchElement(btree *tree, btreeElement *element, int *treeIndex) {
  *treeIndex = -1;
  if (tree->numberOfKeys == 0)
    return tree;
  int middle = 0;
  char found = 0;
  char isLeaf = 0;
  while (!found && !isLeaf) {
    middle = 0;
    int lowerBound = 0;
    int higherBound = tree->numberOfKeys - 1;
    /* TODO: Make binary search */
    while (middle < higherBound &&
           compareBtreeElements(tree->elements[middle], element) < 0)
      middle++;
    short cmpvalue = compareBtreeElements(tree->elements[middle], element);
    if (cmpvalue == 0)
      found = 1;
    else {
      if (cmpvalue < 0)
        middle++;
      btree *newTree = tree->children[middle];
      if (newTree == NULL)
        isLeaf = 1;
      else
        tree = newTree;
      /* When the tree is actually a leaf */
    }
  }
  if (found)
    *treeIndex = middle;
  return tree;
}

void deleteElement(btree *root, btreeElement *element) {
  int treeIndex = 0;
  btree *tree = searchElement(root, element, &treeIndex);
  if (treeIndex > -1) {
    /* Make the element a tombstone */
    free(tree->elements[treeIndex]->value);
    tree->elements[treeIndex]->value = NULL;
  }
}
