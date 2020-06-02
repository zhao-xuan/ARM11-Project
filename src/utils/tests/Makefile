# This Makefile is for testing purpose ONLY

CC = gcc
CFLAGS = -g -Wall -Itestutils -I.. -D_DEFAULT_SOURCE -std=c99 -Werror #-pedantic
LDFLAGS =
LDLIBS = -Ltestutils -ltestutils

utils_test: testutils utils.o utils_test.o
	$(CC) utils.o utils_test.o $(LDFLAGS) $(LDLIBS) -o $@

utils.o: ../utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

utils_test.o: utils_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

testutils:
	cd testutils; make

clean:
	rm -f utils_test *.o core; cd testutils; make clean

.PHONY: clean, testutils
