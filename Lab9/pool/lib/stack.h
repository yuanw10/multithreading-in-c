//The code is from previous labs that I wrote
//Author: me - Yuan Wang
//Date accessed: March 24, 2022



#ifndef __STACK
#define __STACK

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "array_list.h"

typedef struct _Stack
{
  ArrayList* list;
} Stack;


Stack* stack_initialize(int, char*);
bool stack_push(Stack*, void*);
void* stack_pop(Stack*);
void* stack_peek(Stack*);
int stack_size(Stack*);
bool stack_contains(Stack*, void*);
bool stack_destroy(Stack*);


#endif


