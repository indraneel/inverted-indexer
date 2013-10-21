#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

#include "trie.h"
#include "util.h"


//call like this strtolower(string);
void strtolower(char *string) {
    int i, len;
    len = strlen(string);
    for (i=0; i<len; i++) {
	string[i] = tolower(string[i]);	
    }
    return;
}

int compare_token(void *t1, void *t2) {
    return strcmp(((char *)t1), ((char*)t2));
}

int is_file (const char * path) {
    FILE *f = fopen(path, "r");
    if (f) {
	fclose(f);
	return 1;
    }
    else {
	return 0;
    }
}

int is_dir (const char* path) {
    DIR* dir = opendir(path);
    if (dir) {
	closedir(dir);
	return 1;
    }
    else {
	return 0;
    }

}


