//The code is from previous labs that I wrote
//Author: me - Yuan Wang
//Date accessed: March 27, 2022



#ifndef __QUEUE
#define __QUEUE

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "array_list.h"


typedef struct _Queue
{
  ArrayList* list;
} Queue;


Queue* queue_initialize(int, char*);
bool queue_enqueue(Queue*, void*);
void* queue_dequeue(Queue*);
void* queue_peek(Queue*);
int queue_size(Queue*);
bool queue_contains(Queue*, void*);
bool queue_destroy(Queue*);



#endif



