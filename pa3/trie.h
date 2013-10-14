#include <stdio.h>
#include <stdlib.h>
#include "sorted-list.h"

typedef enum {false, true} bool;

typedef struct TrieNode* TrieNodePtr;
struct TrieNode {
    char c;
    TrieNodePtr *parent; 
    TrieNodePtr **children; 
    bool is_word;    
    SortedListPtr list;
};

//Node Methods
TrieNodePtr create_trienode(char c, TrieNodePtr parent);
void destroy_trienode(TrieNodePtr node);

// Tree Methods
TrieNodePtr create_tree();
