TEST_UTILS_DIR = ../../../lib/testutils

CC = gcc
CFLAGS = -g -Wall -I$(TEST_UTILS_DIR) -I../include -D_DEFAULT_SOURCE -std=c99 -Werror #-pedantic
LDFLAGS =
LDLIBS = -L$(TEST_UTILS_DIR) -ltestutils

utils_test: testutils utils.o utils_test.o
	$(CC) utils.o utils_test.o $(LDFLAGS) $(LDLIBS) -o $@

utils.o: ../utils.c ../include/utils.h ../include/global.h
	$(CC) $(CFLAGS) -c -o $@ $<

utils_test.o: utils_test.c ../include/utils.h ../include/global.h
	$(CC) $(CFLAGS) -c -o $@ $<

testutils:
	cd $(TEST_UTILS_DIR); make

clean:
	rm -f utils_test *.o core; 

.PHONY: clean, testutils
