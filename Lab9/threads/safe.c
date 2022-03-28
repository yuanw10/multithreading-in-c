#include "safe.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


void* counting(void* count)
{
  //Set a semaphore wait
  sem_wait(&semaphore);

  Count* countPtr = count;

  //When counter less than 1000
  while(countPtr->counter < 1000)
  {
    //Increment it by 1 and print it out
    countPtr->counter++;
    printf("%d\n", countPtr->counter);
  }
  
  //Lift the semaphore wait
  sem_post(&semaphore);

  return NULL;
}

void count_variable(int threadNum)
{

  //Create a Count
  Count count;

  //Create an array of threads
  pthread_t thread[threadNum];


  //Initialize a semaphore
  sem_init(&semaphore, 0, 1);


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
