#include "threads.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


void* fill(void* args)
{
  Args* arg = args;

  //Squre each value at [start,end)
  for(int i = arg->start; i < arg->end - 1; i++)
  {
    arg->arr[i] *= arg->arr[i];
  }

  //Use a semaphore to avoid race condition
  sem_wait(&semaphore);
  arg->arr[arg->end-1] *= arg->arr[arg->end-1];
  sem_post(&semaphore);

  return NULL;
}

void fill_memory(int* array, int threadNum)
{
  //Create an array of threads
  pthread_t thread[threadNum];

  //Initialize a semaphore
  sem_init(&semaphore, 0, 1);

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
    Args args;
    args.arr = array;
    args.start = arrStart;
    args.end = arrEnd;

    //Create a thread to calculate
    pthread_create(&thread[i], NULL, fill, &args);

  }

  for(int i = 0; i < threadNum; i++)
  {
    pthread_join(thread[i], NULL);

  }
}
