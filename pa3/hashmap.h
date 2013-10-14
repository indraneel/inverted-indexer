#ifndef	HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include "tuple.h"
#include "sorted-list.h"

/*
 * Fergalicious Definitions
 */

struct Record {
    char* filePath;     
    int frequency;
};
typedef struct Record *RecordPtr;

struct HashEntry {
    char* term;
    SortedListPtr records;
};
typedef struct HashEntry *HashEntryPtr;

struct HashMap {
    SortedListPtr list;
};
typedef struct HashMap *HashMapPtr;

/* Record functions */
RecordPtr createRecord(char* path, int count);
int updateRecord(RecordPtr record);
void destroyRecord(RecordPtr record);

/* HashEntry functions */
HashEntryPtr createHashEntry(char* path, int count);
void destroyHashEntry(HashEntryPtr entry);
int addToEntry(HashEntryPtr entry, RecordPtr record);

/* HashMap functions */
HashMapPtr createHashMap();
int addToHash(HashEntryPtr entry);
HashEntryPtr findEntry(char* term);
void destroyHashMap(HashMapPtr hashmap);


#endif
