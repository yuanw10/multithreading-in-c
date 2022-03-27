//The code is from previous labs that I wrote
//Author: me - Yuan Wang
//Date accessed: March 27, 2022


#include "heap.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "array_list.h"


Heap* heap_initialize(int dataTypeSize, char* dataTypeName, 
	              int (*compFunc)(void*, void*), void (*printFunc)(void*))
{
  //Allocate memeory for the new heap
  Heap* newHeap = malloc(sizeof(*newHeap));

  if(newHeap == NULL) return NULL;

  //Set appropriate values to its properties
  newHeap -> list = alist_initialize(100, dataTypeSize, dataTypeName);
  newHeap -> compare = compFunc;
  newHeap -> print = printFunc;

  return newHeap;
}


bool heap_insert(Heap* heap, void* elem)
{
  //If the heap is null, nowhere to insert to 
  if(heap == NULL) return false;

  //Otherwise, add the element to the end of the list
  if(!alist_add(heap -> list, elem)) return false;

  //Shift down the heap to maintain the order
  return _heapify_up(heap, heap -> list -> size - 1);
}


void* heap_remove(Heap* heap)
{
  //If the heap is null, nothing to remove
  if(heap == NULL) return NULL;

  //Swtich the first and the last node on the list
  swap(heap -> list, 0, heap -> list -> size - 1);

  //The root is now at the end of the list
  void* oldRoot = alist_remove(heap -> list, heap -> list -> size - 1);

  //Shift down to maintain the order of the heap
  _heapify(heap, 0);

  return oldRoot;
}


void* heap_peek(Heap* heap)
{
  //If the heap is null, nothing to peek
  if(heap == NULL) return NULL;

  //Get the element at root node
  return alist_get(heap -> list, 0);
}


int heap_size(Heap* heap)
{
  //IF the heap or the list is null, nothing cotained
  if(heap == NULL || heap -> list == NULL) 
    return -1;

  //Get the size of the list
  return heap -> list -> size;
}


bool heap_contains(Heap* heap, void* elem)
{
  if(heap == NULL) return false;

  //If element contained, it is on the list as well
  return alist_index_of(heap -> list, elem) != -1;
}


bool _heapify(Heap* heap, int index)
{
  //Compute the indices of child nodes
  int leIndex = 2 * index + 1;
  int riIndex = 2 * index + 2;

  //Get the current node and its child nodes
  void* curNode = alist_get(heap -> list, index);
  void* leNode = alist_get(heap -> list, leIndex);
  void* riNode = alist_get(heap -> list, riIndex);

  //If the left child is null, bottom reached
  if(leNode == NULL)
  {
    return true;
  }

  //If it has only the left child
  else if(riNode == NULL)
  {
    //If the current node is smaller, shifting down is done
    if(heap -> compare(curNode, leNode) >= 0)
      return true;

    //Otherwise, swap the two nodes, keep shifting down
    else
    {
      swap(heap -> list, leIndex, index);
      return _heapify(heap, leIndex);
    }
  }

  //If it has two child nodes
  else
  {
    //Find out between the node and its child nodes,
    //which one is the smallest
    int top = index;
    if(heap -> compare(alist_get(heap -> list, top), leNode) < 0)
    {
      top = leIndex;
    }
    if(heap -> compare(alist_get(heap -> list, top), riNode) < 0)
    {
      top = riIndex;
    }

    //If the current node is the smallest, shifting down is done
    if(top == index)
      return true;

    //Otherwise, swap two nodes and keep shifting down
    else
    {
      swap(heap -> list, top, index);
      return _heapify(heap, top);
    }
  }
}


bool _heapify_up(Heap* heap, int index)
{
  //Have placed the node at the root
  if(index == 0) return true;

  //Find the parent's index
  int parIndex = (index - 1) / 2;

  //Get the node and its parent
  void* curNode = alist_get(heap -> list, index);
  void* parNode = alist_get(heap -> list, parIndex);

  //Compare the node and its parent
  int compare = heap -> compare(parNode, curNode);

  //If the parent is smaller, the node has been place at
  //a right position. The shifting up is done
  if(compare >= 0)
  {
    return true;
  }

  //Otherwise, keep shifting up
  else
  {
    swap(heap -> list, index, parIndex);
    return _heapify_up(heap, parIndex);
  }
}


//Helper function. Swap two elements on a arraylist
void swap(ArrayList* list, int pos1, int pos2)
{
  //Null and boundary check
  if(list == NULL || list -> arr == NULL || pos1 < 0 || 
     pos1 > list -> size - 1 || pos2 < 0 || pos2 > list -> size - 1)
  return;

  //Swap the elements at the two positions
  void* temp = malloc(sizeof(list -> itemSize));
  memcpy(temp, list -> arr[pos1], list -> itemSize);
  memcpy(list -> arr[pos1], list -> arr[pos2], list -> itemSize);
  memcpy(list -> arr[pos2], temp, list -> itemSize);
}


