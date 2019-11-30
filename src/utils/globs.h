#ifndef GLOBAL_VARS
#define GLOBAL_VARS

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define TIMESTAMP_SIZE 19
#define NUMBER_OF_BTREE_KEYS MAX(2, 2*((CACHESIZE - 18)/80))
/* Our datastructure has 2+8+32*n+8*(n+1) bytes in size, the NUMBER_OF_BTREE_KEYS must be an even number, so this gives the above formula */

#define NOT_IMPLEMENTED_YET_EXIT_CODE 27

/* Define the exit codes for the program */
#define UNKNOWN_COMMAND_EXIT_CODE 5 /* The given command is not known */
#define OUT_OF_MEMORY_EXIT_CODE 12 /* There is no memory anymore */

/* Debug the code */

#endif
