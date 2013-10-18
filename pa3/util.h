#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "trie.h"

typedef enum {false, true} bool;


bool is_file (const char * path);

bool is_dir (const char* path);

#endif
