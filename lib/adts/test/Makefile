CC = gcc
CFLAGS = -Wall -g -Werror -I../include -I../../testutils 
LDLIBS = -L../../testutils -ltestutils 

TEST_FILES = $(wildcard *_test.c)
TEST_OBJ_FILES = $($(TEST_FILES):%.c=%.o)
TEST_BIN_FILES = $(patsubst %_test.c, %_test, $(wildcard *_test.c))  

BUILD = testutils $(TEST_BIN_FILES)

all: $(BUILD)

%_test: %_test.o ../%.o
	$(CC) $^ $(LDLIBS) -o $@

%_test.o: %_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

testutils:
	cd ../../testutils; make

clean:
	rm -f $(LIB) $(TEST_BIN_FILES) *.o core

linked_list.o: include/linked_list.h
