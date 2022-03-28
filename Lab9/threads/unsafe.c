#include "unsafe.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>

void* counting(void* count)
{
  Count* countPtr = count;

  //When counter less than 1000
  while(countPtr->counter < 1000)
  {
    //Increment it bye 1 and print it out
    countPtr->counter++;
    printf("%d\n",countPtr->counter);
  }

  return NULL;
}


void count_variable(int threadNum)
{
  //Create a Count
  Count count;

  //Create an array of threads
  pthread_t thread[threadNum];

  //Create each thread to count
  for(int i = 0; i < threadNum; i++)
  {
    pthread_create(&thread[i], NULL, counting, &count);

  }

  for(int i = 0; i < threadNum; i++)
  {
    pthread_join(thread[i], NULL);

  }

}
