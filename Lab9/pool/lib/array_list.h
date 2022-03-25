//The code below is from the lab solutions written by our TA
//Author: Robert Smith 
//Accessed On: March 25, 2022
//Link: https://git.cs.dal.ca/courses/2022-winter/csci-2122/solutions/-/blob/main/Lab8/heap/lib/array_list.h


#ifndef __ARRAYLIST_HEADER
#define __ARRAYLIST_HEADER
#include <stdbool.h>

typedef struct _AList
{
	void** arr;
	int size;
	int maxSize;
	int itemSize;	
	char* type;	
} ArrayList;

ArrayList* alist_initialize(int, int, char*);
bool       alist_add(ArrayList*, void*);
bool       alist_add_at(ArrayList*, int, void*);
void       alist_clear(ArrayList*);
void*      alist_get(ArrayList*, int);
int        alist_index_of(ArrayList*, void*);
void*      alist_remove(ArrayList*, int);
bool       alist_swap(ArrayList*, int, int);
bool       alist_destroy(ArrayList*);

bool 	   _alist_resize(ArrayList*);

#endif

