# compiler
CC = gcc

# flags
CFLAGS = -g -Wall

all: index

index: indexer.o trie.o tokenizer.o sorted-list.o tuple.o util.o
	$(CC) $(CFLAGS) -o index indexer.o trie.o tokenizer.o sorted-list.o tuple.o util.o

indexer.o: indexer.c indexer.h
	$(CC) $(CFLAGS) -c indexer.c

trie.o: trie.c trie.h
	$(CC) $(CFLAGS) -c trie.c

tokenizer.o: tokenizer.c
	$(CC) $(CFLAGS) -c tokenizer.c

sorted-list.o: sorted-list.c sorted-list.h
	$(CC) $(CFLAGS) -c sorted-list.c

tuple.o: tuple.c tuple.h
	$(CC) $(CFLAGS) -c tuple.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

clean:
	rm -f index
	rm -f *.o 
