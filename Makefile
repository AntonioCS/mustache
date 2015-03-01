#https://gcc.gnu.org/ml/gcc-help/2009-02/msg00130.html


#DO export LD_LIBRARY_PATH=/vagrant_data/Personal/c/mustache to add folder to library path


CC=gcc
SOURCES=$(shell echo src/*.c)
OBJECTS=$(subst src/,obj/,$(SOURCES:.c=.o)) 
#OBJECTS=$(SOURCES:.c=.o)

TARGET=mustache
VERSION=0.1

CFLAGS=-Wall -ggdb3 -std=c99 -Wno-unused-function -o
LCFLAGSOBJ=-std=c99 -fPIC -g -Wall -c

LIBNAME=lib$(TARGET).so.$(VERSION)
LCFLAGS=-shared -Wl,-soname,$(LIBNAME) -o $(LIBNAME)

#tests/tests.c src/mustache.c
#makelib: src/%.c 
#libo: src/mustache.c src/position.c src/strings.c src/tags.c src/text.c src/text_parsed.c src/util.c
#	$(CC) $(CFLAGS) $@ $^

#all: $(OBJECTS)	
#	gcc -std=c99 -shared -W1,-soname,libmustache.so.1 -o libmustache.so.1.0.1 $(OBJECTS)
	#gcc -shared -fPIC -Wl,-soname,libfoo.so.1  -o libfoo.so.1.0.0 foo.c
	
	
#http://www.gnu.org/software/make/manual/make.html#Static-Pattern
#http://www.gnu.org/software/make/manual/make.html#Automatic-Variables	

$(LIBNAME): $(OBJECTS)	
	$(CC) $(LCFLAGS) $^
	
obj/mustache.o: src/mustache.c inc/mustache.h
	$(CC) $(LCFLAGSOBJ) $< -o $@
obj/position.o: src/position.c inc/position.h
	$(CC) $(LCFLAGSOBJ) $< -o $@
obj/position_file.o: src/position_file.c inc/position_file.h
	$(CC) $(LCFLAGSOBJ) $< -o $@
obj/strings.o: src/strings.c inc/strings.h
	$(CC) $(LCFLAGSOBJ) $< -o $@
obj/tags.o: src/tags.c inc/tags.h
	$(CC) $(LCFLAGSOBJ) $< -o $@
obj/text.o: src/text.c inc/text.h
	$(CC) $(LCFLAGSOBJ) $< -o $@
obj/text_file.o: src/text_file.c inc/text_file.h
	$(CC) $(LCFLAGSOBJ) $< -o $@
obj/text_parsed.o: src/text_parsed.c inc/text_parsed.h
	$(CC) $(LCFLAGSOBJ) $< -o $@
obj/util.o: src/util.c inc/util.h
	$(CC) $(LCFLAGSOBJ) $< -o $@

	
#-------------------------------- TESTS Recipies --------------------------------
test_position: tests/test_position.c
	$(CC) $(CFLAGS) $@ $^
	
test_text: tests/test_text.c
	$(CC) $(CFLAGS) $@ $^
	
test_tags: tests/test_tags.c
	$(CC) $(CFLAGS) $@ $^

test_mustache_load_file: tests/test_mustache_load_file.c
	$(CC) $(CFLAGS) $@ $^

test_lib: $(LIBNAME)
	gcc -Wall tests/libtest.c $(shell pwd)/libmustache.so.0.1 -o $@

test_all: test_position test_text test_tags
	./test_position
	./test_text
	./test_tags

#my_parser: main.c src/mustache.c 
#src/mustache_get_contents.c
#	$(CC) $(CFLAGS) $@ $^

clean:
	rm -rf $(LIBNAME)
	rm -rf obj/*.o

.PHONY: all test_position test_text test_tags test_mustache_load_file test_lib
