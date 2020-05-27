# This Makefile is for testing purpose ONLY

CC = gcc
CFLAGS = -g -Wall -Itestutils -I.. -D_DEFAULT_SOURCE -std=c99 #-Werror -pedantic
LDFLAGS =
LDLIBS = -Ltestutils -ltestutils

utilstest: testutils utils.o utilstest.o
	$(CC) utils.o utilstest.o $(LDFLAGS) $(LDLIBS) -o $@

utils.o: ../utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

utilstest.o: utilstest.c
	$(CC) $(CFLAGS) -c -o $@ $<

testutils:
	cd testutils; make

clean:
	rm -f utilstest *.o core; cd testutils; make clean

.PHONY: clean, testutils
