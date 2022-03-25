//The code is from previous labs that I wrote
//Author: me - Yuan Wang
//Date accessed: March 24, 2022

#include "array_list.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

ArrayList* alist_initialize(int maxNum, int typeSize, char* typeName)
{
  //Allocate memory for the list
  ArrayList* list = malloc(sizeof(*list));

  //Initialize values
  list -> maxSize = maxNum;
  list -> itemSize = typeSize;

  list -> type = malloc(sizeof(char) * strlen(typeName));
  strcpy(list -> type, typeName);

  list -> size = 0;
  list -> arr = malloc(maxNum * sizeof(void*));

  return list;
}



bool alist_add(ArrayList* list, void* elem)
{
  if(list == NULL || elem == NULL)
    return false;


  //If max size reached, resize the array
  if(list -> size == list -> maxSize){
    if(!_alist_resize(list))
      return false;
  }

  //Allocate memory for the new element
  list->arr[list->size] = malloc(list->itemSize);

  //Insert the element
  memcpy(list->arr[list->size], elem, list->itemSize);

  //Increase the current size
  list -> size ++;

  return true;
}



bool alist_add_at(ArrayList* list, int index, void* elem)
{
  if(list == NULL || elem == NULL || index > list->size || index < 0)
    return false;


  //If max size reached, resize the array
  if(list -> size == list -> maxSize){
    if(!_alist_resize(list))
      return false;
  }

  //Shift elements on the right of the index to the right by one position
  for(int i = list->size - 1; i >= index; i--)
  {
    list->arr[i+1] = list->arr[i];
  }

  //Allocate memory for the new element
  list->arr[index] = malloc(list->itemSize);

  //Insert the element
  memcpy(list->arr[index], elem, list->itemSize);

  //Increase the current size
  list -> size ++;

  return true;
  
}



void alist_clear(ArrayList* list)
{
  //Free every element on the list
  for(int i = 0; i < list->size; i++)
  {
    free(list->arr[i]);
  }

  //Set current size to zero
  list->size = 0;
}



void* alist_get(ArrayList* list, int index)
{
  //If the index out of range or list null, return null
  if(list == NULL || index > list->size || index < 0)
    return NULL;


  return list->arr[index];
}


int alist_index_of(ArrayList* list, void* elem)
{
  //If empty list or element null, return -1
  if(list == NULL || elem == NULL)
    return -1;

  //Compare every element, if equal, return the index
  for(int i = 0; i < list->size; i++)
  {
    if(!memcmp(elem, list->arr[i], list->itemSize))
      return i;
  }

  //Otherwise, return -1
  return -1;
}


void* alist_remove(ArrayList* list, int index)
{
  //If empty list or index out of range, return null
  if(list == NULL || index < 0 || index > list->size - 1)
    return NULL;

  //Store the element for returning later
  void* elem = list->arr[index];


  //Shift every element right of the index to the left by one position
  for(int i = index + 1; i < list->size; i++)
  {
    list->arr[i - 1] = list->arr[i];
  }


  //Decrease the current size
  list->size--;
  
  //Erase the value at the end of the list
  list->arr[list->size] = NULL;

  return elem;
}


bool alist_destroy(ArrayList* list)
{
  //If empty list, nothing to destroy
  if(list == NULL) return false;

  //Deallocate elements on the list
  for(int i = 0; i < list->size; i++)
  {
    free(list->arr[i]);
  }


  //Deallocate the type and the list itself
  free(list->type);
  free(list);


  return true;
}


bool _alist_resize(ArrayList* list) 
{
  //If empty list or not yet reached max size, return false
  if(list == NULL || list-> size < list->maxSize) 
    return false;


  //Increase the max size by twice
  list->maxSize *= 2;

  //Re-allocate memory for the array
  list->arr = realloc(list->arr, sizeof(void*) * list->maxSize);


  return true;
}




