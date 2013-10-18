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
    TrieNodePtr node = (TrieNodePtr) malloc (sizeof(struct TrieNode));
    node->c = c;
    node->parent = parent;
    node->children = malloc(26*sizeof(struct TrieNode));
    node->is_word = false;
    //node->list = SLCreate(); //needs a function pointer
    return node;
}

void destroy_trienode(TrieNodePtr node) {
    int i;

    if (node==NULL) {
	return;
    }

    for (i=0; i<26; i++) {
	destroy_trienode((node->children[i]));
    }

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

void print_trie(TrieNodePtr node, int depth) {
    int i;
    printf("\n");
    for (i=0; i<26; i++) {
	if ((node->children[i])) {
	    //printf("current node char c = '%c'\n", node->c);
	    //printf("node char c of child #%d = '%c'\n", i, ((node->children[i]))->c);
	    print_trie(node->children[i], depth+1);
	}
    }

    printf("this node = %c\tdepth = %d\n", node->c, depth);
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
    char character, *file_contents, *token;
    FILE *file;
    DIR *dir;
    struct dirent *entry;
    char newname[1024];
    //char *newname = calloc(1024, sizeof(char));
    int num_bytes, pos, converted;
    long filesize;
    TokenizerT *tok;
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
		pos = 0;
		strtolower(token);
		printf("next token: %s", token);
		for (pos=0; pos<strlen(token); pos++) {
		    converted = token[pos] - 97;
		    if (node->children[converted] == NULL) {
			//printf("\ncreating a new node\n");
			node->children[converted] = create_trienode(token[pos], node);
		    }
		    node = node->children[converted];

		    if (node->c != ' ' && (pos == strlen(token)-1)) {
			node->is_word = true;
			node = get_root(node);
		    }
		}
		printf("\n");
		//TODO add to the trie
	    }
	    print_trie(node, 0);
	
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
		while( (token = TKGetNextToken(tok) ) != NULL) {
		    pos = 0;
		    strtolower(token);
		    printf("next token: %s", token);
		    for (pos=0; pos<strlen(token); pos++) {
			converted = token[pos] - 97;
			if (node->children[converted] == NULL) {
			    node->children[converted] = create_trienode(token[pos], node);
			}
			node = node->children[converted];

			if (node->c != ' ' && (pos == strlen(token)-1)) {
			    node->is_word = true;
			    node = get_root(node);
			}
		    }
		}
	    }
	    TKDestroy(tok);
	}
    }
    return;
}

/*
    character = fgetc(file);
    buffer = fgetc(file);

    while(character != EOF) {
	character = tolower(character);	

	if (isalpha(character)) {
	    converted = character - 97;
	    if (ptr->children[converted] == NULL) {
		ptr->children[converted] = create_trienode(character, ptr);
	    }
	    ptr = ptr->children[converted];
	}

	if (ptr != root && (!isalpha(character) || buffer == EOF)) {
	    ptr->is_word = true;
	    ptr = root;
	}
    }
*/
