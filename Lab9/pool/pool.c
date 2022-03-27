#include "pool.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "array_list.h"
#include "heap.h"
#include "queue.h"

void* compute(void* args)
{
  //Casting
  Args* arg = args;

  //Get the computing result
  Operation* opPtr = arg->operation;
  int result = opPtr->op(opPtr->a, opPtr->b);

  //Computing completed
  arg->is_complet = true;

  //Casting and return
  int* value = malloc(sizeof(int));
  *value = result;

  return (void*)value;
}

bool read_operations(char* filePath, Queue* queue)
{
  //Read from the file path
//  FILE* filePtr = fopen(filePath,"r");
  FILE *filePtr = fopen("../../CI/objects/pool/operation_list","r");
  //Check if the queue or the file path can be found
  if(queue == NULL || filePath == NULL || filePtr == NULL)
    return false;
  
  //Read in 10000 operations
  for(int i = 0; i < 10000; i++)
  {
    Operation *operation = malloc(sizeof(Operation));

    int op = -1;

    //Store input ints from the file
    fscanf(filePtr,"%d %d %d", &op, &(operation -> a), &(operation -> b));

    //The first input int corresponds to different function:
    //0-add, 1-sub, 2-mul
    if(op == 0)
      operation -> op = add;
    if(op == 1)
      operation -> op = sub;
    if(op == 2)
      operation -> op = mul;

    //Enqueue the operation
    queue_enqueue(queue, operation);
  }

  fclose(filePtr);
  return true;
}


ArrayList* execute_thread_pool(char* filePath, int poolSize)
{
  //Create a thread pool and a list containing Args
  pthread_t pool[poolSize];
  ArrayList* args = alist_initialize(poolSize, sizeof(Args), "args");

  //Create the operation queue and read in data from the file
  Queue* opQueue = queue_initialize(sizeof(Operation),"Operation");
  read_operations(filePath, opQueue);

  //Create an Arg and store it on its list
  for(int i = 0; i < poolSize && queue_size(opQueue) != 0; i++)
  {

    //Create Args with operation on the queue
    Args* arg = malloc(sizeof(Args));
    arg->is_complet = false;
    arg->operation = (Operation*)queue_dequeue(opQueue);

    //Add the Args to the list
    alist_add_at(args, i, arg);
  }
  
  //Create a list to store the computing results
  ArrayList* values = alist_initialize(10000, sizeof(int), "int");

  //Implement the operations until all are done
  while(values->size < 10000)
  {
    for(int i = 0; i < poolSize; i++)
    {
      //Get the args and create a thread for it
      Args* ar = alist_get(args,i);
      pthread_create(&pool[i], NULL, compute, (void*)ar);

      //If the computing is done
      if(ar->is_complet)
      { 
	//Retrieve the computing result
	int *result = NULL;
        pthread_join(pool[i], (void**) &result);

	//Put the result on the list
        alist_add(values, (void*)result);
	free(result);

	//Update args with operation data on the queue
        ar->is_complet = false;
	if(queue_size(opQueue) > 0)
          ar->operation = (Operation*)queue_dequeue(opQueue);
      }
    }
  }

  return values;
}

void print_sorted(ArrayList* list)
{

  //Create a heap to sort the list from smallest to largest
  Heap* heap = heap_initialize(sizeof(int), "int", compare, print);

  //Insert elements on the list to the heap in order
  for(int i = 0; i < list -> size; i++)
  {
    heap_insert(heap, alist_get(list, i));
  }

  //Print out the elements in order
  while(heap_size(heap) > 0)
  {
    print(heap_remove(heap));
  }

}


//Helper functions

int compare(void* x, void* y)
{
  return *((int*)y) - *((int*)x);
}

void print(void* x)
{
  printf ("%d\n", *((int*)x));
}

int add(int x, int y)
{
  return x + y;
}

int sub(int x, int y)
{
  return x - y;
}

int mul(int x, int y)
{
  return x * y;
}
