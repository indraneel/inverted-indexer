# compiler
CC = gcc

# flags
CFLAGS = -g -Wall

all: index
	
index: indexer.c indexer.h util.h util.c trie.c trie.h tokenizer.c
	$(CC) $(CFLAGS) -o index indexer.c trie.c util.c


trie: trie.c trie.h
	$(CC) $(CFLAGS) -o trie trie.c trie.h

tokenizer: tokenizer.c
	$(CC) $(CFLAGS) -o tokenizer tokenizer.c

clean:
	$(RM) index *.o *~
