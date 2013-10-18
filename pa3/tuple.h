#ifndef	TUPLE_H
#define TUPLE_H

#include <stdlib.h>

struct Tuple
{
    char* fileName;
    int count;
};
typedef struct Tuple* TuplePtr;

#endif
