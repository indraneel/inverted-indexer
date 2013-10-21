#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>

#include "trie.h"
#include "tuple.h"
#include "sorted-list.h"
#include "tokenizer.h"
#include "util.h"

TrieNodePtr create_trienode(char c, TrieNodePtr parent) {
    //int i;
    TrieNodePtr node = (TrieNodePtr) malloc (sizeof(struct TrieNode));
    if (!node) {
	fprintf(stderr, "Malloc for node with char c = %c and parent c = %c failed\n",c, parent->c);
	exit(-1);
    }
    node->c = c;
    node->parent = parent;
    node->children = (TrieNodePtr*)calloc(36,sizeof(TrieNodePtr));
    node->is_word = false;
    node->word = NULL;
    /*for (i=0; i<36; i++) {
	memset(node->children[i], 0, sizeof(TrieNodePtr));
    }*/
    if (c=='q') {
	printf("q\n");
    }
    return node;
}

void destroy_trienode(TrieNodePtr node) {
    int i;

    if (node==NULL) {
	return;
    }

    for (i=0; i<36; i++) {
	if (!(node->children[i]) || !(node->children[i]->c)) {
	    continue;
	}
	destroy_trienode((node->children[i]));
    }

    if (node->is_word && node->list) {
	SLDestroy(node->list);
    }
    else { 
	//SLDestroy(node->list);
	node->list = 0;
    }
    //free(node->children);
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

int write_to_file(TrieNodePtr node, FILE *output, SortedListPtr tokenlist) {
    int i, count, found, first; 
    count = 0;
    found = 0;
    SortedListIteratorPtr iter, tokeniter;
    void *next;
    TuplePtr tuple;

    if (!node){
	free(node);
	return found;
    }
    if (node->children) {
	for (i=0; i<36; i++) {
	    if (node->children[i]) {
		write_to_file(node->children[i], output, tokenlist );
	    }
	}
    }
    

    if (node->is_word && node->list) {
	found = 1;
	count = 0;
	first = 1;
	iter = SLCreateIterator(node->list);
	//while ((tuple = (TuplePtr)SLNextItem(iter)) != NULL) {
	while (count < 5 && ((next= SLNextItem(iter)) != NULL)) {
	    tuple = (TuplePtr) next;
	    if (first == 1) {
		fprintf(output, "<list> %s\n %s %d", node->word, ((TuplePtr) next)->fileName, ((TuplePtr) next)->count);
	    }

	    else {
		fprintf(output, " %s %d", ((TuplePtr) next)->fileName, ((TuplePtr) next)->count);
	    }
	    
	    count = count+1;
	    first = 0;
	}
	SLDestroyIterator(iter);
    }
    if (found ==1 ) {
	fprintf(output, "\n</list>\n");
    }
	
    return found;
}

void print_trie(TrieNodePtr node, int depth) {
    int i;
    if (!node) return;
    if (!node->children) return;
    for (i=0; i<36; i++) {
	if (!(node->children[i]) || !(node->children[i]->c)) {
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
    TuplePtr tuple, temp;
    void *item;

    for (pos=0; pos<strlen(token); pos++) {
	item = NULL;
	//is not alpha
	int converted = convert_char(token[pos]);
	if (converted == -1) { continue; }
	if (node->children[converted] == NULL) {
	    node->children[converted] = create_trienode(token[pos], node);
	}
	node = node->children[converted];

	//if we reach the end of the token, it's a word
	if (node->c != ' ' && (pos == strlen(token)-1)) {
	    if (!node->list) {
		printf("creating list\n");
		CompareFuncT func = &compare_tuple;
		node->list = SLCreate(func); //needs a function pointer
		//SLPrint(node->list);
	    }
	    else {
		//SLPrint(node->list);
	    }
	    node->is_word = true;
	    node->word = (char *) malloc(strlen(token) + 1);
	    strcpy(node->word, token);
	    printf("node->word = %s @ %p\n", node->word, node->word); 
	    //tuple = (TuplePtr) malloc(sizeof(struct Tuple));
	    tuple = create_tuple();
	    if (!tuple) {
		fprintf(stderr, "Tuple failed to be malloc'd\n");
		return 1;
	    }
	    ((TuplePtr)tuple)->fileName = (char*) malloc(strlen(path) + 1);
	    if (!((TuplePtr)tuple)->fileName) {
		fprintf(stderr, "filename for tuple %p did not malloc\n", tuple);
		return 1;
	    }
	    strcpy(((TuplePtr)tuple)->fileName, path);
	    ((TuplePtr)tuple)->count = 1;
	    printf("current token = %s @ %p\n", token, token);
	    printf("tuple just created->filename = %s @ %p\n", ((TuplePtr)tuple)->fileName, ((TuplePtr)tuple));

	    SortedListIteratorPtr iter = SLCreateIterator(node->list);
	    if (!iter) {
		fprintf(stderr, "SLIterator failed to be created\n");
		return 1;
	    }
	    item = SLNextItem(iter);
	    while(item) {
		if (((TuplePtr)item)->fileName) {
		//it matched something in list, update count and break
		    if (strcmp( (((TuplePtr)item)->fileName), path) == 0) {
			//printf("there's a dupe!\n");	
			//(((TuplePtr)item)->count) = (((TuplePtr)item)->count)+1;
			(((TuplePtr)tuple)->count) = (((TuplePtr)item)->count)+1;
			SLRemove(node->list, item);
			found_word = 1;
			break;
		    }
		}
		    //printf("tuple: {%s,%d}", ((TuplePtr)item)->fileName, ((TuplePtr)item)->count);
		    item = SLNextItem(iter);
		    if(item)
			    printf("->");
	    }
	    //printf("\n\n");

	    SLInsert(node->list, tuple); 
	    if (found_word == 0) {
		//printf("inserting tuple: {%s,%d}", tuple->fileName, tuple->count);
		//SLInsert(node->list, tuple); 
	    }

	    SLDestroyIterator(iter);
	    //free(tuple->fileName);
	    //free(tuple);		
	    node = get_root(node);
	}
    }
    //printf("\n");
    return 0;
}
void index_file(TrieNodePtr node, char *filename, SortedListPtr tokenlist) {
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
    fread(file_contents, sizeof(char), nmemb, file);
    fclose(file);

    tok = TKCreate(file_contents);
    if (tok) {
	token = NULL;
	printf("\nabout to add toke = %s @ %p to trie\n", token, token);
	while ((token = TKGetNextToken(tok)) != NULL) {
	    strtolower(token);
	    SLInsert(tokenlist, token);
	    add_to_trie(node, token, filename);
	}
	token = NULL;
	free(token);
	TKDestroy(tok);
    }
    else {
	fprintf(stderr, "Error: Tokenizer not properly created.\n");
    }
    free(file_contents);
}

void index_dir(TrieNodePtr node, char *dirpath, SortedListPtr tokenlist) {
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
		index_dir(node, name, tokenlist);
	    }

	    else if (is_file(name)) {
		index_file(node, name, tokenlist);
	    }
	}
	closedir(dir);
    }
}

