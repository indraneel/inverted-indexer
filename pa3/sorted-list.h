#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include "tuple.h"

typedef int (*CompareFuncT)(TuplePtr *, TuplePtr*);

typedef struct Node *NodePtr;
struct Node
{
	void *obj;
	unsigned int referenceCount;
	unsigned int removed;

	NodePtr next;
};

struct SortedList
{
	NodePtr head;

	CompareFuncT compare;
};
typedef struct SortedList *SortedListPtr;


struct SortedListIterator
{
	unsigned int started;
	NodePtr curr;
	SortedListPtr list;
};
typedef struct SortedListIterator *SortedListIteratorPtr;


////////////////////////////////////////
// List Functions
////////////////////////////////////////
SortedListPtr SLCreate(CompareFuncT cf);
void SLDestroy(SortedListPtr list);
int SLInsert(SortedListPtr list, void *newObj);
int SLRemove(SortedListPtr list, void *newObj);

////////////////////////////////////////
// Iterator Functions
////////////////////////////////////////
SortedListIteratorPtr SLCreateIterator(SortedListPtr list);
void SLDestroyIterator(SortedListIteratorPtr iter);
void *SLNextItem(SortedListIteratorPtr iter);

#endif
