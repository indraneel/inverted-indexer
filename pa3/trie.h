#ifndef TRIE_H
#define TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "sorted-list.h"


typedef struct TrieNode* TrieNodePtr;
struct TrieNode {
    char c;
    TrieNodePtr parent; 
    TrieNodePtr *children; 
    bool is_word;    
    //SortedListPtr list;
};

//Node Methods
TrieNodePtr create_trienode(char c, TrieNodePtr parent);
void destroy_trienode(TrieNodePtr node);

// Tree Methods
TrieNodePtr create_trie();
void build_trie(TrieNodePtr node, char *path);

#endif
