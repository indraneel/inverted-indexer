#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "trie.h"

TrieNodePtr create_trienode(char c, TrieNodePtr parent) {
    TrieNodePtr node = (TrieNodePtr) malloc (sizeof(struct TrieNode));
    node->c = c;
    node->parent = parent;
    node->children = malloc(26*sizeof(struct TrieNode));
    node->is_word = false;
    node->list = SLCreate(); //needs a function pointer
    return node;
}

void destroy_trienode(TrieNodePtr node) {
    int i;

    if (node==NULL) {
	return;
    }

    for (i=0; i<26; i++) {
	destroy_trienode(node->children[i]);
    }

    free(node->children);
    free(node);
    return;
}

TrieNodePtr create_tree(FILE *file) {
    TrieNodePtr root = create_trienode(' ', NULL);    
    TrieNodePtr ptr = root;
    int character;
    int converted;
    int buffer;

    character = fgetc(file);
    buffer = fgetc(file);

    /* rip this out */
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

    return root;
}
