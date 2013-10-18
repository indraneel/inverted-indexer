#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "tuple.h"
#include "trie.h"

typedef enum {false, true} bool;


int compare_tuple(TuplePtr t1, TuplePtr t2);

bool is_file (const char * path);

bool is_dir (const char* path);

#endif
