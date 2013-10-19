#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <string.h>

#include "trie.h"
#include "tuple.h"
#include "sorted-list.h"
#include "tokenizer.c"

TrieNodePtr create_trienode(char c, TrieNodePtr parent) {
    /*printf("char c for this trienode = %c\n", c);
    if (parent) {
	printf("char c for the parent  = %c\n", parent->c);
    }*/
    CompareFuncT func = &compare_tuple;
    TrieNodePtr node = (TrieNodePtr) malloc (sizeof(struct TrieNode));
    node->c = c;
    node->parent = parent;
    node->children = malloc(36*sizeof(struct TrieNode));
    node->is_word = false;
    node->list = SLCreate(func); //needs a function pointer
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

/*TODO
 * change param to initial path name
 *
 */
TrieNodePtr create_trie() {
    TrieNodePtr root = create_trienode(' ', NULL);    
    TrieNodePtr ptr = root;
    int character;
    int converted;
    int buffer;
    return root;
}

TrieNodePtr get_root(TrieNodePtr node) {
    while (node->parent != NULL) {
	node = node->parent;
    }
    return node;
}

//void print_trie(TrieNodePtr node, int depth, char *outbuf) {
void print_trie(TrieNodePtr node, int depth) {
    int i;
    //printf("\n");
    if (!node) return;
    if (!node->children) return;
    for (i=0; i<36; i++) {
	if (!(node->children[i])) {
	    continue;
	}
	if ((node->children[i])) {
	    //printf("current node char c = '%c'\n", node->c);
	    //printf("node char c of child #%d = '%c'\n", i, ((node->children[i]))->c);
	    print_trie(node->children[i], depth+1);
	}
    }
    
    if (node->is_word) {
	//printf("this node = %s\tdepth = %d\n", node->word, depth);
	printf("%s => [", node->word);
	SortedListIteratorPtr iter = SLCreateIterator(node->list); 
	void *item;	
	item = SLNextItem(iter);
	//printf("\niterating over node->list\n");
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

//call like this strtolower(string);
void strtolower(char *string) {
    int i, len;
    len = strlen(string);
    //char *output = (char *) malloc(len*sizeof(char));
    for (i=0; i<len; i++) {
	string[i] = tolower(string[i]);	
    }
    //return output;
    return;
}

void build_trie(TrieNodePtr node, char *path) {
    char character, *file_contents, *token;//, *output;
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
			if (!node->children[converted]) {
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

