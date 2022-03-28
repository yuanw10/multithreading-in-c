#include "threads.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


void* fill(void* args)
{
  Args* arg = args;

  //Squre each value at [start,end)
  for(int i = arg->start; i < arg->end; i++)
  {
    arg->arr[i] *= arg->arr[i];
  }

  return NULL;
}

void fill_memory(int* array, int threadNum)
{
  //Create an array of threads
  pthread_t thread[threadNum];

  //Divide the array size
  int range = 10000000 / threadNum;

  for(int i = 0; i < threadNum; i++)
  {
    //Compute indexes of the array
    int arrStart = i * range;
    int arrEnd = -1;

    //For the last part, set end as the last index
    if ( i = threadNum - 1)
      arrEnd = 10000000;
    else 
      arrEnd = arrStart + range;

    //Create an Args to store the array and proper indexes
    Args* args = malloc(sizeof(Args));
    args->arr = array;
    args->start = arrStart;
    args->end = arrEnd;

    //Create a thread to calculate
    pthread_create(&thread[i], NULL, fill, args);

  }

  for(int i = 0; i < threadNum; i++)
  {
    pthread_join(thread[i], NULL);

  }
}
