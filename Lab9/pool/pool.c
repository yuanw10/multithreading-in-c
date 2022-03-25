#include "pool.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "array_list.h"
#include "bintree.h"
#include "queue.h"

void* compute(void* arg)
{
  //Casting
  Args args = *(Args*)(arg);

  //Get the computing result
  Operation* opPtr = args.operation;
  int result = opPtr->op(opPtr->a, opPtr->b);

  //Computing cmpleted
  args.is_complet = true;

  //Casting and return
  void* value = &result;
  return value;
}

bool read_operations(char* filePath, Queue* queue)
{
  //Read from the file path
  FILE *filePtr = fopen(filePath,"r");

  //Check if the queue or the file path can be found
  if(queue == NULL || filePath == NULL || filePtr == NULL)
    return false;
  
  //Read in operations
  for(int i = 0; i < 10000; i++)
  {
    Operation *operation;

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

  return true;
}


ArrayList* execute_thread_pool(char* filePath, int poolSize)
{
  //Create a thread pool and a list containing Args
  ArrayList* pool = alist_initialize(poolSize, sizeof(pthread_t), "thread");
  ArrayList* args = alist_initialize(poolSize, sizeof(Args), "args");

  //Create the operation queue and read in data from the file
  Queue* opQueue = queue_initialize(sizeof(Operation),"Operation");
  read_operations(filePath, opQueue);
  
  //Create threads for the pool
  for(int i = 0; i < poolSize && queue_size(opQueue) != 0; i++)
  {
    pthread_t thread;

    //Create Args with operation on the queue
    Args arg;
    arg.is_complet = false;
    arg.operation = (Operation*)queue_dequeue(opQueue);
    
    //Add the thread and corresponding args to the lists
    void* tPtr = &thread;
    void* aPtr = &arg;
    alist_add(pool, tPtr);
    alist_add(args, aPtr);

    //Create a thread
    pthread_create(&thread, NULL, compute, &arg);
  }
  
  //Create a list to store the computing results
  ArrayList* values = alist_initialize(10000, sizeof(int), "int");


  //Dequeue the operation queue until it is empty
  while(queue_size(opQueue) != 0)
  {
    for(int i = 0; i < poolSize; i++)
    {
      //If the computing is done
      Args ar = *(Args*)(alist_get(args,i));
      if(ar.is_complet)
      { 
	//Retrieve the computing result
	int *result = NULL;
        pthread_join(*(pthread_t*)alist_get(pool,i), (void**) &result);

	//Put the result to the list
        alist_add(values, result);

	//Update args with operation data on the queue
        ar.is_complet = false;
        ar.operation = (Operation*)queue_dequeue(opQueue);

	//Create new thread
        alist_remove(pool, i);
        pthread_t newThread;
	alist_add_at(pool, i, &newThread);
        pthread_create(&newThread, NULL, compute, alist_get(args,i));
      }
    }
  }

  return values;
}

void print_sorted(ArrayList* list)
{
  //Use binary tree to store the values
  BinaryTree *btree = bintree_initialize(sizeof(int), "int", compare, print);

  //Use the results to create a binary tree
  for(int i = 0; i < list -> size; i++)
  {
    bintree_insert(btree, alist_get(list,i));
  }

  //Print the tree in-order (smallest to largest)
  bintree_print_in_order(btree);
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

