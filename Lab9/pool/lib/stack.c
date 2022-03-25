//The code is from previous labs that I wrote
//Author: me - Yuan Wang
//Date accessed: March 24, 2022



#include "stack.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "array_list.h"

Stack* stack_initialize(int typeSize, char* typeName)
{
  //Allocate memory for the stack
  Stack* stack = malloc(sizeof(stack));

  //Initialize the list inside the stack, set max size as 100
  stack -> list = alist_initialize (100, typeSize, typeName);

  return stack;
}


bool stack_push(Stack* stack, void* elem)
{
  //Empty stack, return false
  if(stack == NULL) return false;


  //Add the element to the end of the list
  return alist_add(stack -> list, elem);
}


void* stack_pop(Stack* stack)
{
  //If empty stack, nothing to pop
  if(stack == NULL) return false;

  //Remove the last element on the list inside the stack
  return alist_remove(stack -> list, stack -> list -> size - 1);
}

void* stack_peek(Stack* stack)
{
  //If empty stack, nothing to peek
  if(stack == NULL) return false;

  //Get the last element on the list inside the stack
  return alist_get(stack -> list, stack -> list -> size - 1);
}


int stack_size(Stack* stack)
{
  //If empty stack, no elements
  if(stack == NULL || stack -> list == NULL)
    return 0;

  return stack -> list -> size;
}

bool stack_contains(Stack* stack, void* elem)
{
  //If empty stack, nothing contained
  if(stack == NULL)
    return false;

  //Check if the element on the list inside the stack
  //If contained, the index is not -1
  return alist_index_of(stack -> list, elem) != -1;
}

bool stack_destroy(Stack* stack)
{
  //If empty stack, nothing to deallocate
  if(stack == NULL)
    return false;

  //Destroy the list inside the stack
  alist_destroy(stack -> list);

  //Deallocate the stack
  free(stack);

  return true;
}



