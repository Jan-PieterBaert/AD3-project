#ifndef BTREE
#define BTREE

#define TIMESTAMP_SIZE 19
#define NUMBER_OF_BTREE_KEYS 10

typedef struct btree {
  struct btree *children[NUMBER_OF_BTREE_KEYS+1];
  char* keys[NUMBER_OF_BTREE_KEYS];
  char timestamp[TIMESTAMP_SIZE];
  unsigned int tombstone : 1;
  char *value;
} btree;

btree *allocateBtree();
btree *addToBtree(btree *root, char timestamp[], char *value);

#endif
