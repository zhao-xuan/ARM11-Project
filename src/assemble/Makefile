BIN_DIR = ../../bin
OBJ_DIR = ../../obj/assemble
UTIL_OBJ_DIR = ../../obj/common
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

ADT_UTIL_DIR = ../../lib/adts

CC      = gcc
IncludePath = -I../../lib/include -I../common/ -I../common/include -Iinclude
CFLAGS  = -O $(IncludePath) -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
LDFLAGS = -L$(ADT_UTIL_DIR) -lADT
BUILD = libs $(BIN_DIR)/assemble

all: $(BUILD)

$(BIN_DIR)/assemble: $(OBJ_FILES) $(UTIL_OBJ_DIR)/utils.o
	$(CC) $^ $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(UTIL_OBJ_DIR)/utils.o: ../common/utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

libs:
	cd ../../lib; make

clean:
	rm -f $(wildcard $(OBJ_DIR)/*.o)
	rm -f $(wildcard $(UTIL_OBJ_DIR)/*.o)
	rm -f $(BIN_DIR)/assemble
	cd tests; make clean

.SUFFIXES: .c .o .h
.PHONY: clean test
