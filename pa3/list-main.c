/*
 * sorted-list.c
 */

#include <string.h>
#include <stdio.h>
#include "sorted-list.h"

int compareInts(void *p1, void *p2)
{
	int i1 = *(int*)p1;
	int i2 = *(int*)p2;

	return i1 - i2;
}

int compareDoubles(void *p1, void *p2)
{
	double d1 = *(double*)p1;
	double d2 = *(double*)p2;

	return (d1 < d2) ? -1 : ((d1 > d2) ? 1 : 0);
}

int compareStrings(void *p1, void *p2)
{
	char *s1 = p1;
	char *s2 = p2;

	return strcmp(s1, s2);
}

int main()
{

	CompareFuncT func = &compareInts;
	int *item;
	int element;
	/////////////////////////////////


	/////////////////////////////////
	// NORMAL CASES
	/////////////////////////////////

	// CREATE A LIST
	SortedListPtr list = SLCreate(func);
	if(!list) {
		return 0;
	}

	// ADD ELEMENTS TO A LIST
	int i;
	int ints[] = {5, 3, 7, 0, 9};
	for(i = 0; i < sizeof(ints)/sizeof(int); i++) {
		SLInsert(list, &ints[i]);
	}

	// REMOVE 3 FROM LIST
	element = 3;
	SLRemove(list, &element);

	// CREATE AN ITERATOR
	SortedListIteratorPtr iter = SLCreateIterator(list);

	// CREATES LIST:  EMPTY
	// ADDS ELEMENT:  9->7->5->3->0
	// REMOVE 3:      9->7->5->0
	printf("NORMAL CASES OUTPUT:\n");
	item = SLNextItem(iter);
	while(item) {
		printf("%d", *item);
		item = SLNextItem(iter);
		if(item)
			printf("->");
	}
	printf("\n\n");

	SLDestroyIterator(iter);
	SLDestroy(list);

	/////////////////////////////////
	// EDGE CASES
	/////////////////////////////////

	// CREATE A LIST
	func = &compareDoubles;
	list = SLCreate(func);
	if(!list) {
		return 0;
	}

	// ADD ELEMENTS TO LIST
	double doubles[] = {5.0, 3.0, 7.0};
	for(i = 0; i < sizeof(doubles)/sizeof(double); i++) {
		SLInsert(list, &doubles[i]);
	}

	// CREATE ITERATOR
	iter = SLCreateIterator(list);
	double *next;

	next = SLNextItem(iter); // 7.0
	printf("%f\n", *next);
	next = SLNextItem(iter); // 5.0
	printf("%f\n", *next);
	double four = 4.0;
	SLInsert(list, &four);
	next = SLNextItem(iter); // 4.0
	printf("%f\n", *next);
	SLRemove(list, &four);
	SLInsert(list, &four);
	next = SLNextItem(iter); // 4.0
	printf("%f\n", *next);
	SLRemove(list, &four);
	double five = 5.0;
	SLInsert(list, &five);
	next = SLNextItem(iter); // 3.0
	printf("%f\n", *next);
	double two = 2.0;
	SLInsert(list, &two);
	// LIST IS NOW 7.0->5.0->5.0->3.0->2.0

	// PRINT LIST, SHOULD BE 7.0->5.0->5.0->3.0->2.0
	SortedListIteratorPtr iter2 = SLCreateIterator(list);
	next = SLNextItem(iter2);
	while(next) {
		printf("%f", *next);
		next = SLNextItem(iter2);
		if(next)
			printf("->");
	}
	printf("\n\n");
	SLDestroyIterator(iter2);


	iter2 = SLCreateIterator(list);
	SLNextItem(iter2);
	SLNextItem(iter2);
	SLNextItem(iter2);
	SLNextItem(iter2);
	// BOTH ITERATORS ARE NOW POINTING TO 3.0

	// REMOVE 3.0
	double three = 3.0;
	SLRemove(list, &three);

	next = SLNextItem(iter);
	printf("%f\n", *next);
	next = SLNextItem(iter2);
	printf("%f\n", *next);


	SLDestroyIterator(iter2);
	SLDestroyIterator(iter);
	SLDestroy(list);




	/*-
	// GETTING THE FIRST ITEM FROM A LIST
	item = SLNextItem(iter2);
	printf("%d\n", *item);
	item = SLNextItem(iter2);
	printf("%d\n", *item);

	// REMOVE ELEMENT WITH VALUE OF 5
	SLRemove(list, &ints[1]);

	// GET NEXT, SHOULD BE 3
	item = SLNextItem(iter2);
	printf("%d\n", *item);

	// GET NEXT, SHOULD BE 2
	int a = 2;
	SLInsert(list, &a);
	item = SLNextItem(iter2);
	if(item) {
		printf("%d\n", *item);
	}

	// GET NEXT, SHOULD BE NULL
	a = 4;
	SLInsert(list, &a);
	item = SLNextItem(iter2);
	if(item) {
		printf("%d\n", *item);
	}*/

	////////////////////////


	return 1;

}
