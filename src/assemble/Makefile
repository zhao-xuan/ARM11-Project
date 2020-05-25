BIN_DIR = ../../bin
OBJ_DIR = ../../obj/assemble
UTIL_OBJ_DIR = ../../obj/utils

SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

CC      = gcc
CFLAGS  = -I../../lib -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror #-pedantic
LDFLAGS = 


$(BIN_DIR)/assemble: $(OBJ_FILES) $(UTIL_OBJ_DIR)/utils.o
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(UTIL_OBJ_DIR)/utils.o: ../utils/utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(wildcard $(OBJ_DIR)/*.o)
	rm -f $(wildcard $(UTIL_OBJ_DIR)/*.o)
	rm -f $(BIN_DIR)/assemble

.SUFFIXES: .c .o .h
.PHONY: clean