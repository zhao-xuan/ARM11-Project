# This Makefile is for testing purpose ONLY

CC = gcc
CFLAGS = -g -Wall -Itestutils -I../include -D_DEFAULT_SOURCE -std=c99 #-Werror -pedantic
LDFLAGS =
LDLIBS = -Ltestutils -ltestutils
BUILD = logicunit_test

all: testutils $(BUILD)

logicunit_test: logicunit.o logicunit_test.o
	$(CC) $^ $(LDFLAGS) $(LDLIBS) -o $@

logicunit.o: ../logicunit.c
	$(CC) $(CFLAGS) -c -o $@ $<

logicunit_test.o: logicunit_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

testutils:
	cd testutils; make

clean:
	rm -f logicunit_test *.o core; cd testutils; make clean 

.PHONY: clean, testutils

