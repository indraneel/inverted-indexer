#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include "trie.h"
#include "util.h"

int compare_tuple(TuplePtr t1, TuplePtr t2) {
    return ((t1->count)-(t2->count));
}

bool is_file (const char * path) {
    FILE *f;
    if (!(f = fopen(path, "r"))) {
	return false;
    }
    fclose(f);
    return true;
}

bool is_dir (const char* path) {
    DIR* dir = opendir(path);
    if (!dir) {
	return false;
    }
    closedir(dir);
    return true;
}


