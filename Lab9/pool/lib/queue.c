//The code is from previous labs that I wrote
//Author: me - Yuan Wang
//Date accessed: March 24, 2022


#include "queue.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "array_list.h"


Queue* queue_initialize(int typeSize , char* typeName)
{
  //Allocate memory for the queue
  Queue* queue = malloc(sizeof(queue));


  //Create a list inside the queue, set max size as 10000
  queue -> list = alist_initialize (10000, typeSize, typeName);

  return queue;
}


bool queue_enqueue(Queue* queue, void* elem)
{
  if(queue == NULL) return false;
  
  //Add the element to list inside the queue
  return alist_add(queue -> list, elem);
}


void* queue_dequeue(Queue* queue)
{ 
  //If empty queue, nothing to dequeue
  if(queue == NULL) return false;
  
  //Remove the first element on list inside the queue
  return alist_remove(queue -> list, 0);
}

void* queue_peek(Queue* queue)
{
  //If empty queue, nothing to peek
  if(queue == NULL) return false;
  
  //Get the first element on list inside the queue
  return alist_get(queue -> list, 0);
}

int queue_size(Queue* queue)
{ 
  //If empty queue or empty list, return 0
  if(queue == NULL || queue -> list == NULL)
     return 0;
  
  return queue -> list -> size;
}

bool queue_contains(Queue* queue, void* elem)
{ 
  if(queue == NULL || elem == NULL)
     return false;
  
  //Get the index of the element on the list
  //If the index is not -1, then it is contained
  return alist_index_of(queue -> list, elem) != -1;
}

bool queue_destroy(Queue* queue)
{
  //If empty queue, nothing to deallocate
  if(queue == NULL) return false;
  
  //Otherwise deallocate the list inside the queue
  alist_destroy(queue -> list);

  //Then deallocate the queue itself
  free(queue);

  return true;
}



