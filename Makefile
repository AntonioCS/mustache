CFLAGS=-Wall -g -std=c99 -Wno-unused-function -o
CC=gcc


m_tests: tests/tests.c src/mustache.c 
	$(CC) $(CFLAGS) $@ $^

#my_parser: main.c src/mustache.c 
#src/mustache_get_contents.c
#	$(CC) $(CFLAGS) $@ $^

clean:
	rm -rf m_tests
