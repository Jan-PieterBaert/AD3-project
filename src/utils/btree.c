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
  /* TEMPORAL IMPLEMENTATION */
  int depth = 1;
  btree *t = root;
  if (t->numberOfKeys == 0)
    depth = 0;
  else {
    while (t->children[0] != NULL) {
      t = t->children[0];
      depth++;
    }
  }
  int *path = path = (int *)malloc((depth + 1) * sizeof(int));
  for (int i = 0; i < depth; i++)
    path[i] = -1;
  searchElement(root, element, &path);
  t = root;
  int currentDepth = 0;
  if (depth != 0) {
    int pathElement = path[0];
    while (pathElement != -1 && currentDepth < depth-1) {
      t = t->children[pathElement];
      currentDepth++;
      pathElement = path[currentDepth];
    }
  }
  if (t->numberOfKeys < NUMBER_OF_BTREE_KEYS) {
    int index = t->numberOfKeys;
    int i = 0;
    i = path[currentDepth];
    if (i < index && strcmp(t->elements[i]->key, element->key) < 0)
      i++;
    /* When the key is already in the database, update the value*/
    if (i < index && strcmp(t->elements[i]->key, element->key) == 0) {
      free(t->elements[i]->value);
      t->elements[i]->value = element->value;
    } else {
      for (int j = index; j > i; j--)
        t->elements[j] = t->elements[j - 1];
      t->elements[i] = element;
      t->numberOfKeys++;
    }
  } else {
      int insertIndex = path[0];
      printf("InsertIndex: %d\n", insertIndex);
  }
#ifdef DEBUG
  printf("Printing all keys, amount: %d\n", t->numberOfKeys);
  for (int j = 0; j < t->numberOfKeys; j++) {
    printf("%s\n", t->elements[j]->key);
  }
  printf("\n");
#endif

  free(path);
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

btreeElement *searchElement(btree *tree, btreeElement *element, int **path) {
  if (tree->numberOfKeys == 0)
    return NULL;
  int *pathList = NULL;
  if (path != NULL)
    pathList = *path;
  int index = 0;
  int middle = 0;
  char found = 0;
  char isLeaf = 0;
  while (!found && !isLeaf) {
    int lowerBound = 0;
    int higherBound = tree->numberOfKeys - 1;
    while (lowerBound + 4 < higherBound) {
      middle = lowerBound + (higherBound - lowerBound) / 2;
      short cmpValue = strcmp(tree->elements[middle]->key, element->key);
      if (cmpValue > -1)
        higherBound = middle;
      if (cmpValue < 1)
        lowerBound = middle;
    }
    middle = lowerBound;
    while (middle < higherBound &&
           strcmp(tree->elements[middle]->key, element->key) < 0)
      middle++;
    if (strcmp(tree->elements[middle]->key, element->key) == 0)
      found = 1;
    else {
      btree *newTree = tree->children[middle];
      if (newTree == NULL)
        isLeaf = 1;
      else
        tree = newTree;
      /* When the tree is actually a leaf */
    }
    if (pathList != NULL)
      pathList[index] = middle;
    index++;
#ifdef DEBUG
    printf("INDEX: %d\n", index);
#endif
  }
  if (!found) {
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
