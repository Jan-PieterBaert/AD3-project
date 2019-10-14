#include "btree.h"
#include <stdio.h>
#include <string.h>

#include "misc.h"

btreeElement *allocateBtreeElement() {
  btreeElement *retval = nullSafeMalloc(sizeof(btreeElement));
  retval->value = NULL;
  return retval;
}

int compareBtreeElements(btreeElement *e1, btreeElement *e2) {
  return strcmp(e1->key, e2->key);
}

void freeBtreeElement(btreeElement *tree) {
  if (tree->value != NULL) {
    free(tree->value);
  }
  free(tree);
}

btree *allocateBtree() {
  btree *retval = nullSafeMalloc(sizeof(btree));
  retval->numberOfKeys = 0;
  return retval;
}

btree *addToBtree(btree *root, btreeElement *element) {
  int currentSize = BTREE_PATH_SIZE;
  int *path = nullSafeMalloc(currentSize * sizeof(int));
  btree **treesOnPath = nullSafeMalloc(currentSize * sizeof(btree *));
  path[0] = -1;
  treesOnPath[0] = root;
  int index = 0;
  btree *tree = root;
  char isLeaf = 0;
  while (!isLeaf) {
    treesOnPath[index] = tree;
    int lowerBound = 0;
    int higherBound = tree->numberOfKeys;
    int middle = 0;
    while (lowerBound != higherBound) {
      middle = higherBound - (higherBound - lowerBound) / 2;
      if (strcmp(tree->elements[middle]->key, element->key) < 0) {
        higherBound = middle;
      } else {
        lowerBound = middle;
      }
    }
    middle = lowerBound;
    index++;
    if (index == currentSize - 1) {
      currentSize += BTREE_PATH_SIZE;
      path = realloc(path, currentSize * sizeof(int));
      treesOnPath = realloc(treesOnPath, currentSize * sizeof(btree *));
    }
    treesOnPath[index] = tree;
    path[index] = middle;
    btree *newTree = tree->children[middle];
    if (newTree == NULL)
      isLeaf = 1;
    else
      tree = newTree;
    /* When the tree is actually a leaf */
  }
  int middle = path[index];
  tree = treesOnPath[index];
  btreeElement *newValue = addElementToBtreeLeaf(&tree, element, middle);
  while (newValue != NULL && index > 0) {
    tree = treesOnPath[index];
    middle = path[index];
    btreeElement *newValue = addElementToBtreeChild(&tree, element, middle);
    index--;
  }
  if (newValue != NULL) {
    /* Make new tree from child, split */
  }
  free(path);
  free(treesOnPath);
  return root;
}

btreeElement *addElementToBtreeLeaf(btree **_tree, btreeElement *element,
                                    int index) {
  btree *tree = *_tree;
  if (tree->numberOfKeys < NUMBER_OF_BTREE_KEYS) {
    for (int i = tree->numberOfKeys; i > index; i--) {
      tree->elements[i] = tree->elements[i - 1];
    }
    tree->elements[index] = element;
    tree->numberOfKeys++;
  }
  return NULL;
}

btreeElement *addElementToBtreeChild(btree **tree, btreeElement *element,
                                     int index) {
  return NULL;
}

btreeElement *searchElement(btree *tree, btreeElement *element) {
  int middle = 0;
  char found = 0;
  char isLeaf = 0;
  while (!found && !isLeaf) {
    int lowerBound = 0;
    int higherBound = tree->numberOfKeys - 1;
    while (lowerBound != higherBound) {
      middle = lowerBound + (higherBound - lowerBound) / 2;
      if (strcmp(tree->elements[middle]->key, element->key) < 0) {
        higherBound = middle;
      } else {
        lowerBound = middle;
      }
    }
    middle = lowerBound;
    if (strcmp(tree->elements[middle]->key, element->key)==0)
      found = 1;
    else {
      btree *newTree = tree->children[middle];
      if (newTree == NULL)
        isLeaf = 1;
      else
        tree = newTree;
      /* When the tree is actually a leaf */
    }
  }
  if(!found){
      return NULL;
  }
  element = tree->elements[middle];
  return element;
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
