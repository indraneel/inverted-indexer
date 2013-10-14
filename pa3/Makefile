# compiler
CC = gcc

# flags
CFLAGS = -g -Wall

all: index
	
index: index.o
	$(CC) $(CFLAGS) -o index index.o

index.o: indexer.c indexer.h
	$(CC) $(CFLAGS) 	

trie: trie.o
	$(CC) $(CFLAGS) -o trie trie.c trie.h

clean:
	$(RM) index *.o *~
