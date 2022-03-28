#ifndef __SAFE_HEADER
#define __SAFE_HEADER

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


typedef struct _Count
{
  int counter;
} Count;


sem_t semaphore;

void* counting(void*);
void count_variable(int);



#endif
