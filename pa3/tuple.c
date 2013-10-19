#include "tuple.h"

int compare_tuple(TuplePtr t1, TuplePtr t2) {
    return ((t1->count)-(t2->count));
}
