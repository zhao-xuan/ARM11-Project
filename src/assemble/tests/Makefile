# This Makefile is for testing purpose ONLY

CC = gcc
IncludePath = -I../../../lib -I../../../lib/include -I../../common/ -I../../common/include -I../include -Itestutils
CFLAGS = -g -Wall $(IncludePath) -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
LDFLAGS = 
LDLIBS = -Ltestutils -ltestutils -L../../../lib -lADT

TEST_FILES = $(wildcard *_test.c)
BUILD = $(patsubst %_test.c, %_test, $(TEST_FILES))

SRC_FILES = $(wildcard ../*.c)
SRC_NO_MAIN = $(filter-out ../assemble.c, $(SRC_FILES))
OBJ_FILES = $(patsubst ../%.c, %.o,$(SRC_NO_MAIN)) utils.o

all: testutils $(BUILD)

%_test: %_test.o $(OBJ_FILES)
	$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@

%_test.o: %_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: ../%.c
	$(CC) $(CFLAGS) -c -o $@ $<

utils.o: ../../common/utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

testutils:
	cd testutils; make

clean:
	rm -f $(BUILD) *.o core
	rm -f memory_test.txt
	cd testutils; make clean 

.PHONY: clean, testutils

