#ifndef __THREADS_HEADER
#define __THREADS_HEADER


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct _Args
{
  int* arr;
  int start;
  int end;
} Args;


void* fill(void*);
void fill_memory(int*, int);



#endif
