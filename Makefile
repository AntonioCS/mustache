CFLAGS=-Wall -g -std=c99 -o
CC=gcc

my_parser: main.c
	$(CC) $(CFLAGS) $@ $^

clean:
	rm -rf my_parser