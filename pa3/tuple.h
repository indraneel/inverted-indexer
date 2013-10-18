#ifndef	TUPLE_H
#define TUPLE_H

#include <stdlib.h>


/*
 * Represents < "/path/to/file", # of occurrences of a word in that file>
 */
struct Tuple
{
    char* fileName;
    int count;
};
typedef struct Tuple* TuplePtr;

#endif
