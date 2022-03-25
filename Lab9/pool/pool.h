#ifndef __POOL_HEADER
#define __POOL_HEADER

#include "pool.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "array_list.h"
#include "bintree.h"
#include "queue.h"


typedef struct _Operation
{
  int (*op)(int, int);
  int a;
  int b;
} Operation;


typedef struct _Args
{
  Operation* operation;
  bool is_complet;
} Args;



void* compute(void*);
bool read_operations(char*, Queue*);
ArrayList* execute_thread_pool(char*, int);
void print_sorted(ArrayList*);
int compare(void* , void*);
void print(void*);
int add(int, int);
int sub(int, int);
int mul(int, int);


#endif
