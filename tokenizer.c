/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {false, true} bool;

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

struct TokenizerT_ {
    char *sep;
    char *tokens_string;
    bool *ascii_hash;
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate(char *separators, char *ts) {
    int i;
    printf("TKCreate\n---\n");
    /* allocate space for TokenizerT struct */
    TokenizerT *tokenizer = (TokenizerT *) malloc(sizeof(TokenizerT));
    if (tokenizer == NULL) {
	printf("woops\n");
	return NULL;
    }
    
    tokenizer->sep = (char *)malloc(sizeof(strlen(separators)+1));
    if (tokenizer->sep == NULL) {
	printf("woops\n");
	return NULL;
    }
    strcpy(tokenizer->sep, separators);

    tokenizer->tokens_string = (char *)malloc(sizeof(strlen(ts)+1));
    if (tokenizer->tokens_string == NULL) {
	printf("woops\n");
	return NULL;
    }
    strcpy(tokenizer->tokens_string, ts);
    printf("tokensstring = %s\n",(tokenizer->tokens_string));
    
    tokenizer->ascii_hash = (bool *) calloc(128, sizeof(bool));
    if (tokenizer->ascii_hash == NULL) {
	printf("woops\n");
	return NULL;
    }
    else {
	printf("ascii_hash values:\n");
	for (i=0; i<128; i++) {
	    printf("%d",tokenizer->ascii_hash[i]);
	}
	printf("\n----\n");
    }
   

    /* if fail, return 
    return NULL;*/
    printf("tokenizer struct address: %p\n", tokenizer);
    printf("end TKCreate\n---\n");
    return tokenizer;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
    free(tk->sep);
    tk->sep = NULL;
    free(tk->tokens_string);
    tk->tokens_string= NULL;
    free(tk);
    tk = NULL;
    return;
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {

    return NULL;
}

void generateSeparatorHash(char* word, TokenizerT* tok) {
    printf("whole sep string = %s\n", word);
    int i, j=0;

    bool escape = false;
    char * currChar = NULL;
    for (i=0; i<strlen(word); i++) {
	/* previous character was a \ */
	if (escape == true) {
	    escape = false;
	}
	/* previous character wasn't a \ */
	else {
	    if (word[i] == '\\') {
		printf("escape sequence\n"); 
		escape = true; 
		currChar = (char *)word[i];
		printf("escape => %c\n", currChar);
		continue;
	    }
	    else {
		escape = false;
	    }
	    printf("@%d => %c", i, word[i]);
	    printf(" or ");
	    /*printf("%d\n", (unsigned int)word[i]);*/
	    printf("%d\n", word[i]-0);
	    tok->ascii_hash[word[i]-0] = true;
	}
    }
}

void print_ascii_hash(bool* ascii_hash){
    int i;
    printf("ascii_hash values:\n");
    for (i=0; i<128; i++) {
	printf("%d",ascii_hash[i]);
	if (ascii_hash[i] == true) {
	    printf("(%d)",i);
	}
    }
    printf("\n----\n");
}

void splitByToken(char* tokenString) {
    int i=0;
    while (tokenString[i] != '\0') {
	printf("%c\n", tokenString[i]); 
	i++;
    }
}


/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {

    if (argc != 3) {
	fprintf(stderr, "tokenizer - error, incorrect # of arguments.\nUsage: tokenizer [separator characters] [tokens string]\n");
	return 1;
    }

    else {
	//create tokenizer here
	printf("program name = %s\n", argv[0]);
	printf("separator chars = %s\n", argv[1]);
	TokenizerT *tk = TKCreate(argv[1], argv[2]);
	printf("add %p\n", tk);
	printf("tk->sep=%s\n",tk->sep);
	printf("tk->ts=%s\n",tk->tokens_string);
	generateSeparatorHash(argv[1], tk);
	print_ascii_hash(tk->ascii_hash);
	printf("tokens = %s\n", argv[2]);
	splitByToken(argv[2]);
	TKDestroy(tk);
    }
    
    return 0;
} 
