#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include "sorted-list.h"
#include "tuple.h"
#include "trie.h"
#include "util.h"

int main(int argc, char** argv) {
	int result;
	FILE *output;
	if (argc != 3) {
		fprintf(stderr, "Error: Improper amount of arguments\nUsage: index <inverted-index file name> <directory or file name>\n");	
		return 1;
	}	

	/*if(argv[2][strlen(argv[2])-1] == '/') {
	    fprintf(stderr, "Error: Please don't tab-complete directory names.\n");
	    printf("Usage index <inverted-index> <directory name with no trailing '/' or file name>\n");
	    return 1;*
	}	*/
    

	if (strcmp(argv[1],argv[2])==0) {
	    fprintf(stderr, "Output and input file cannot be the same.\n");
	    return 1;
	}

	if (access(argv[2], F_OK)==-1) {
	    fprintf(stderr, "Output file already exists.\n");
	    return 1;
	}
	
	output = fopen(argv[1], "w");

	if (!output) {
	    fprintf(stderr, "Error: output file was not created\n");
	    return 1;
	}
	
	TrieNodePtr root = create_trie();
	
	if (is_dir(argv[2]) == 1) {
	    printf("This is a directory\n");
	    index_dir(root, argv[2]);
	    //add_dir_to_trie
	    //
	}
	else if (is_file(argv[2]) == 1) {
	    printf("This is a file\n"); 
	    index_file(root, argv[2]);
	}

	else {
	    fprintf(stderr, "Error: %s is not a file or directory.\n", argv[2]);
	    fclose(output);
	    return 1;
	}
	
	//build_trie(root, argv[2]);
	
	//save trie to file
	print_trie(root, 0);
	//printf("about to call write_to_file\n");
	//result = write_to_file(root, output);
	//printf("call complete write_to_file\n");
	/*if (result == 1) {
	    printf("something shoulda printed\n"); 
	}*/
	destroy_trienode(root);	
	fclose(output);
	return 0;
}
