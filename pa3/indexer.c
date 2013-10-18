#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "sorted-list.h"
#include "tuple.h"
#include "trie.h"
#include "util.h"

int main(int argc, char** argv) {

	if (argc != 3) {
		fprintf(stderr, "Error: Improper amount of arguments\nUsage: index <inverted-index file name> <directory or file name>\n");	
		return 1;
	}	

	if(argv[2][strlen(argv[2])-1] == '/') {
	    fprintf(stderr, "Error: Please don't tab-complete directory names.\nUsage index <inverted-index> <directory name with no trailing '/' or file name>\n");
	    return 1;
	}	

	/*
	FILE *f;
	f = fopen(argv[1], "r");
	if (f) {
	    printf("This file already exists\nPlease choose a new name for your output file.\n"); 
	    fclose(f);
	    return;
	}
	*/
	TrieNodePtr root = create_trie();
	build_trie(root, argv[2]);

	/*	
	if (is_dir(argv[2])) {
	    DIR *dirptr= opendir (argv[2]);
	    struct dirent *direntptr;
	    char* name;
	    while ((direntptr = readdir(dirptr)) != NULL) {
		if (strcmp(direntptr->d_name, "..")==0 || strcmp(direntptr->d_name,".")==0) {
		}
		else {
		    printf ("[%s]\tis file? %d\n", direntptr->d_name, is_file(direntptr->d_name)); 
		}
	    }
	    closedir(dirptr);
	}
	*/
	
	return 0;
}
