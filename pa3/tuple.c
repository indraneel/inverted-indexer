#include "tuple.h"
#include <string.h>

int compare_tuple(TuplePtr t1, TuplePtr t2) {
    int result;
    result = (t2->count)-(t1->count);
    return result == 0 ? strcmp( t1->fileName, t2->fileName ) : result;
}

TuplePtr create_tuple()  {
    TuplePtr out = (TuplePtr) malloc(sizeof(struct Tuple));
    if (out) {
	out->fileName = NULL;
	out->count = 0;
	return out;
    }
    else {
	return NULL;
    }
}
