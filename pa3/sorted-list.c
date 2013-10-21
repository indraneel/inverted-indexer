#include <stdio.h>
#include "sorted-list.h"

////////////////////////////////////////
// Big-O Analysis
////////////////////////////////////////
/*	
	Where n is the size of the list:
	 	Insert:  O(n) worst case, O(n/2) average
	 	Remove:
	 		Failure:
	 			O(n) worst case, O(n/2) average
	 		Success:
	 			O(n) worst case, O(n/2) average
	 	Destroy: O(n) worst case and average
*/

////////////////////////////////////////
// List Functions
////////////////////////////////////////
SortedListPtr SLCreate(CompareFuncT cf) {

	// Allocate memory for sorted list structure
SortedListPtr sl = malloc(sizeof(struct SortedList));

	// Initialize list variables
	sl->head = NULL;
	sl->compare = cf;

	return sl;
}

void SLPrint(SortedListPtr list) {

	// Free list nodes and then free list
	NodePtr curr = NULL, next = list->head;
	while(next != NULL) {
		curr = next;
		printf("%p\n", curr);
		next = curr->next;
	}
}

void SLDestroy(SortedListPtr list) {

	// Free list nodes and then free list
	NodePtr curr = NULL, next = list->head;
	while(next != NULL) {
		curr = next;
		next = curr->next;
		free(curr);
	}
	free(list);
}



int SLInsert(SortedListPtr list, void *newObj) {

	// Create node
	NodePtr node = malloc(sizeof(struct Node));
	node->obj = newObj;
	node->referenceCount = 1;
	node->next = NULL;
	node->removed = 0;

	// List is empty
	if(list->head == NULL) {
		list->head = node;
		return 1;
	}

	NodePtr curr = list->head, prev = NULL;
	while(curr != NULL) {
		// Keep looking
		if(list->compare(node->obj, curr->obj) > 0) {
			prev = curr;
			curr = curr->next;
		}
		// Insert before curr
		else {
			node->next = curr;
			if(prev == NULL) {
				// Head of list
				list->head = node;
			}
			else {
				// Normal case
				prev->next = node;
			}
			return 1;
		}
	}
	prev->next = node;
	return 1;
}

int SLRemove(SortedListPtr list, void *newObj) {

	NodePtr curr = list->head, prev = NULL;
	int comparison;
	while(curr != NULL) {
		comparison = list->compare(newObj, curr->obj);
		if(comparison > 0) {
			// Element definitely can't be in sorted list
			break;
		}
		else if(comparison == 0) { 
			if(prev == NULL) {
				// Head of list
				list->head = curr->next;
			}
			else {
				// Normal case
				prev->next = curr->next;
			}
			curr->referenceCount--;
			curr->next = NULL;
			curr->removed = 1;
			if(curr->referenceCount == 0) {
				free(curr);
			}
			return 1;
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}
	return 0;
}

////////////////////////////////////////
// Iterator Functions
////////////////////////////////////////
SortedListIteratorPtr SLCreateIterator(SortedListPtr list) {
	
	SortedListIteratorPtr iter = malloc(sizeof(struct SortedListIterator));

	// iterator should also store a pointer to the list itself to get the head once it starts iterating over elements
	iter->started = 0;
	iter->curr = NULL;
	iter->list = list;

	return iter;
}

void SLDestroyIterator(SortedListIteratorPtr iter) {

	// update reference counts if still pointing to an element!
	// make sure curr is still member of list, if not, free that node
	if(iter->curr) {
		iter->curr->referenceCount--;
		if(iter->curr->referenceCount == 0) {
			free(iter->curr);
		}
	}
	
	free(iter);
}

void *SLNextItem(SortedListIteratorPtr iter) {
	
	if(iter->curr) {
		// need to check if node we're leaving has been deleted, if so, different procedure
		if(iter->curr->removed) {
			// loop through list from head and find where next element would be based on 
			// on value of iter->curr->obj
			// if this is last iterator leaving this element, don't forget to free it
			NodePtr node = iter->list->head;
			while(node != NULL) {
				if(iter->list->compare(node->obj, iter->curr->obj) <= 0) {
					// we've found our next element
					break;
				}
				else {
					node = node->next;
				}
			}

			iter->curr->referenceCount--;
			if(iter->curr->referenceCount == 0) {
				free(iter->curr);
			}

			iter->curr = node;
			if(node == NULL) {
				return NULL;
			}
			iter->curr->referenceCount++;
			return iter->curr->obj;
		}
		else {
			// Normal procedure
			NodePtr prev = iter->curr;
			iter->curr = iter->curr->next;
			prev->referenceCount--;

			// Return object contained in next node
			if(iter->curr) {
				iter->curr->referenceCount++;
				return iter->curr->obj;
			}
			else {
				return NULL;
			}

		}
	}
	else {
		// Either iteration hasn't started, or we're at the end of the list
		if(iter->started) {
			return NULL;
		}
		else {
			if(iter->list->head) {
				iter->curr = iter->list->head;
				iter->curr->referenceCount++;
				iter->started = 1;
				return iter->curr->obj;
			}
			else{
				return NULL;
			}
			
		}
	}

}

