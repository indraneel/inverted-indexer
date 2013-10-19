#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>

#include "trie.h"
#include "tuple.h"
#include "sorted-list.h"
#include "tokenizer.c"
#include "util.h"

TrieNodePtr create_trienode(char c, TrieNodePtr parent) {
    TrieNodePtr node = (TrieNodePtr) malloc (sizeof(struct TrieNode));
    node->c = c;
    node->parent = parent;
    node->children = malloc(36*sizeof(struct TrieNode));
    node->is_word = false;
    return node;
}

void destroy_trienode(TrieNodePtr node) {
    int i;

    if (node==NULL) {
	return;
    }

    for (i=0; i<36; i++) {
	destroy_trienode((node->children[i]));
    }

    SLDestroy(node->list);
    free(node->children);
    free(node);
    return;
}

TrieNodePtr create_trie() {
    TrieNodePtr root = create_trienode(' ', NULL);    
    return root;
}

TrieNodePtr get_root(TrieNodePtr node) {
    while (node->parent != NULL) {
	node = node->parent;
    }
    return node;
}

int write_to_file(TrieNodePtr node, FILE *output) {

}

void print_trie(TrieNodePtr node, int depth) {
    int i;
    if (!node) return;
    if (!node->children) return;
    for (i=0; i<36; i++) {
	if (!(node->children[i])) {
	    continue;
	}
	if ((node->children[i])) {
	    print_trie(node->children[i], depth+1);
	}
    }
    
    if (node->is_word) {
	printf("%s => [", node->word);
	SortedListIteratorPtr iter = SLCreateIterator(node->list); 
	void *item;	
	item = SLNextItem(iter);
	while(item) {
	    //it matched something in list, update count and break
	    printf("{%s,%d}", ((TuplePtr)item)->fileName, ((TuplePtr)item)->count);
	    item = SLNextItem(iter);
	    if(item)
		    printf(", ");
	}
	SLDestroyIterator(iter);
	printf("]\n");
    }
    return;
}



/*
 * Subtracts 97 from letters to index a at 0 and z and 25
 * Starts indexing numbers at 26 (0 at 26)
 * -1 if anything else makes it way in
 */
int convert_char(const char c) {
    if (isalpha(c)) {
	return (int) (c - 'a');
    }
    else if (isdigit(c)) {
	return (int) (c - '0')+26;	
    }
    else {
	return -1;
    }
}

int add_to_trie(TrieNodePtr node, char *token, char *path) {
    int found_word = 0;
    int pos = 0;
    TuplePtr tuple;
    void *item;

    for (pos=0; pos<strlen(token); pos++) {
	item = NULL;
	//is not alpha
	int converted = convert_char(token[pos]);
	if (node->children[converted] == NULL) {
	    node->children[converted] = create_trienode(token[pos], node);
	    CompareFuncT func = &compare_tuple;
	    node->list = SLCreate(func); //needs a function pointer
	}
	node = node->children[converted];

	//if we reach the end of the token, it's a word
	if (node->c != ' ' && (pos == strlen(token)-1)) {
	    node->is_word = true;
	    node->word = (char *) malloc(strlen(token) + 1);
	    strcpy(node->word, token);
	    
	    tuple = (TuplePtr) malloc(sizeof(struct Tuple));
	    if (!tuple) {
		fprintf(stderr, "Tuple failed to be malloc'd\n");
		return 1;
	    }
	    tuple->fileName = (char*) malloc(strlen(path) + 1);
	    strcpy(tuple->fileName, path);
	    tuple->count = 1;

	    SortedListIteratorPtr iter = SLCreateIterator(node->list);
	    if (!iter) {
		fprintf(stderr, "SLIterator failed to be created\n");
		return 1;
	    }
	    item = SLNextItem(iter);
	    while(item) {
		//it matched something in list, update count and break
		if (strcmp((((TuplePtr)item)->fileName), path) == 0) {
		    //printf("there's a dupe!\n");	
		    (((TuplePtr)item)->count) = (((TuplePtr)item)->count)+1;
		    found_word = 1;
		}
		    //printf("tuple: {%s,%d}", ((TuplePtr)item)->fileName, ((TuplePtr)item)->count);
		    item = SLNextItem(iter);
		    if(item)
			    printf("->");
	    }
	    //printf("\n\n");

	    if (found_word == 0) {
		//printf("inserting tuple: {%s,%d}", tuple->fileName, tuple->count);
		SLInsert(node->list, tuple); 
	    }

	    SLDestroyIterator(iter);
	    free(tuple);		
	    node = get_root(node);
	}
    }
    //printf("\n");
}

void index_file(TrieNodePtr node, const char *filename) {
    FILE *file;
    TokenizerT *tok;
    char *file_contents, *token;
    size_t nmemb;

    file = fopen(filename, "r");
    if (!file) {
	fprintf(stderr, "Error: %s could not be opened for indexing.\n", filename);
    }

    //Copy all of the file into a string
    fseek(file, 0, SEEK_END);
    nmemb = ftell(file);
    rewind(file);
    file_contents = (char *) malloc(nmemb * sizeof(char));
    fclose(file);

    tok = TKCreate(file_contents);
    if (tok) {
	while ((token = TKGetNextToken(tok)) != NULL) {
	    strtolower(token);
	    add_to_trie(node, token, filename);
	}
	TKDestroy(tok);
    }
    else {
	fprintf(stderr, "Error: Tokenizer not properly created.\n");
    }
}

void index_dir(TrieNodePtr node, const char *dirpath) {
    DIR *dir;    
    struct dirent *entry;

    dir = opendir(dirpath);
    if (!dir) {
	fprintf(stderr, "Directory '%s' did not open\n:", dirpath);
    }
    else {
	//wamedoomadirwalk
	while ( (entry = readdir(dir)) ) {
	    size_t size = strlen(entry->d_name) + strlen(dirpath) + 2;
	    char *name = calloc(size, sizeof(char));
	    snprintf(name, size, "%s/%s", dirpath, entry->d_name);
	    
	    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
		continue;
	    }

	    else if (is_dir(name)) {
		index_dir(node, name);
	    }

	    else if (is_file(name)) {
		index_file(node, name);
	    }
	}
	closedir(dir);
    }
}

/*
void build_trie(TrieNodePtr node, char *path) {
    char *file_contents, *token;//, *output;
    FILE *file;
    DIR *dir;
    struct dirent *entry;
    char newname[1024];
    //char *newname = calloc(1024, sizeof(char));
    int num_bytes, pos, converted;
    long filesize;
    void *item;
    bool found = false;
    TokenizerT *tok;
    TuplePtr tuple;

    //if (is_file(path)) {
  
    //not dir
    if (!(dir = opendir(path))) {
	file = fopen(path, "r");
	if (!file) return;
	//TODO CaLL TOKENIZER
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	rewind(file);
	file_contents= (char*) calloc(filesize+1,sizeof(char));
	fread(file_contents, sizeof(char), filesize, file);
	fclose(file);

	printf("%s\n\n", file_contents);
	tok = TKCreate(file_contents);
	//tok = TKCreate(x);
	if (tok) {
	    while( (token = TKGetNextToken(tok) ) != NULL) {
		found = false;
		pos = 0;
		strtolower(token);
		printf("next token: %s", token);
		for (pos=0; pos<strlen(token); pos++) {
		    //is not alpha
		    if ( isalpha(token[pos]) == 0 ) {
			converted = token[pos];
		    }
		    else {
			converted = token[pos] - 87;
		    }
		    if (node->children[converted] == NULL) {
			//printf("\ncreating a new node\n");
			node->children[converted] = create_trienode(token[pos], node);
		    }
		    node = node->children[converted];

		    if (node->c != ' ' && (pos == strlen(token)-1)) {
			node->is_word = true;
			node->word = (char*) calloc(strlen(token), sizeof(char));
			strcpy(node->word, token);
			
			tuple = (TuplePtr) malloc(sizeof(struct Tuple));
			tuple->fileName = (char*) calloc(strlen(path), sizeof(char));
			strcpy(tuple->fileName, path);
			tuple->count = 1;


			SortedListIteratorPtr iter = SLCreateIterator(node->list);
			item = SLNextItem(iter);
			//printf("\niterating over node->list\n");
			while(item) {
			    //it matched something in list, update count and break
			    if (strcmp((((TuplePtr)item)->fileName), path) == 0) {
				printf("there's a dupe!\n");	
				(((TuplePtr)item)->count) = (((TuplePtr)item)->count)+1;
				found = true;
			    }
				printf("tuple: {%s,%d}", ((TuplePtr)item)->fileName, ((TuplePtr)item)->count);
				item = SLNextItem(iter);
				if(item)
					printf("->");
			}
			printf("\n\n");

			if (!found) {
			    printf("inserting tuple: {%s,%d}", tuple->fileName, tuple->count);
			    SLInsert(node->list, tuple); 
			}

			SLDestroyIterator(iter);
			    
			node = get_root(node);
		    }
		}
		printf("\n");
		//TODO add to the trie
	    }
	    //output = (char*) malloc(sizeof(char)*10240);
	    //print_trie(node, 0, output);
	    print_trie(node, 0);
	    //TODO save to file
	    destroy_trienode(node);
	}
	TKDestroy(tok);
	return;
    }

    while ((entry = readdir(dir))) {
	if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0) continue;
	printf("this should be a dir: %s\n", path);
	
	//it's a directory! recurse
	if (entry->d_type == DT_DIR) {
	    num_bytes = snprintf(newname, sizeof(newname), "%s/%s", path, entry->d_name);
	    build_trie(node, newname);

	}

	//it's a file! run it through tokenizer
	if (entry->d_type == DT_REG) {
	    num_bytes = snprintf(newname, sizeof(newname), "%s/%s", path, entry->d_name);
	    file = fopen(newname, "r");
	    if (!file) return;
	    //TODO cal tokenizer
	    fseek(file, 0, SEEK_END);
	    filesize = ftell(file);
	    rewind(file);
	    file_contents= (char*) calloc(filesize+1,sizeof(char));
	    fread(file_contents, sizeof(char), filesize, file);
	    fclose(file);

	    printf("%s\n\n", file_contents);
	    tok = TKCreate(file_contents);
	    //tok = TKCreate(x);
	    if (tok) {
		token = TKGetNextToken(tok);
		if (!token) return;
		//while( (token = TKGetNextToken(tok) ) != NULL) {
		do {
		    pos = 0;
		    strtolower(token);
		    printf("next token: %s", token);
		    for (pos=0; pos<strlen(token); pos++) {
			//is not alpha
			if (isalpha(token[pos]) == 0) {
			    converted = token[pos];
			}
			else {
			    converted = token[pos] - 87;
			}
			if (node->children[converted] == NULL) {
			    //printf("\ncreating a new node\n");
			    node->children[converted] = create_trienode(token[pos], node);
			}
			node = node->children[converted];

			if (node->c != ' ' && (pos == strlen(token)-1)) {
			    node->is_word = true;
			    node->word = (char*) calloc(strlen(token)+1, sizeof(char));
			    strcpy(node->word, token);
			    
			    tuple = (TuplePtr) malloc(sizeof(struct Tuple));
			    tuple->fileName = (char*) calloc(strlen(path)+1, sizeof(char));
			    strcpy(tuple->fileName, path);
			    tuple->count = 1;


			    SortedListIteratorPtr iter = SLCreateIterator(node->list);
			    item = SLNextItem(iter);
			    printf("\niterating over node->list\n");
			    while(item) {
				//it matched something in list, update count and break
				if (strcmp((((TuplePtr)item)->fileName), path) == 0) {
				    printf("there's a dupe!\n");	
				    (((TuplePtr)item)->count) = (((TuplePtr)item)->count)+1;
				    found = true;
				}
				    printf("tuple: {%s,%d}", ((TuplePtr)item)->fileName, ((TuplePtr)item)->count);
				    item = SLNextItem(iter);
				    if(item)
					    printf("->");
			    }
			    printf("\n\n");

			    if (!found) {
				printf("inserting tuple: {%s,%d}", tuple->fileName, tuple->count);
				SLInsert(node->list, tuple); 
			    }



			    node = get_root(node);
			}
		    }
		}
		while( (token = TKGetNextToken(tok) ) != NULL);
		//output = (char*) malloc(sizeof(char)*10240);
		//print_trie(node, 0, output);
		print_trie(node, 0);
	    }
	    TKDestroy(tok);
	}
    }
    return;
}
*/

