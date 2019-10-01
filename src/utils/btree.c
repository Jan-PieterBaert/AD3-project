#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"

btree* allocateBtree(){
  btree *retval = NULL;
  while (retval == NULL) {
    retval = malloc(sizeof(btree));
  }
  return retval;
}

btree* addToBtree(btree* root, char timestamp[], char* value){
    
}
