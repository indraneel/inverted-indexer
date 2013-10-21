#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "tuple.h"
#include "trie.h"

typedef enum {false, true} bool;


void strtolower(char *string);

int compare_token(void *t1, void *t2);

int is_file (const char * path);

int is_dir (const char* path);

#endif
