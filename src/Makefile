# SRC_DIR = ../src
OBJ_DIR = ../obj
BIN_DIR = ../bin

# SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
# OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

CC      = gcc
CFLAGS  = -I../lib -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 #-Werror -pedantic
LDFLAGS = 
BUILD=$(BIN_DIR)/assemble $(BIN_DIR)/emulate

all: $(BUILD)

$(BIN_DIR)/assemble: $(OBJ_DIR)/assemble.o
	$(CC) $(LDFLAGS) -o $@ $^

$(BIN_DIR)/emulate: $(OBJ_DIR)/emulate.o $(OBJ_DIR)/utils.o $(OBJ_DIR)/print.o $(OBJ_DIR)/decode.o $(OBJ_DIR)/execute.o
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/assemble.o: assemble.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/emulate.o: emulate.c utils.h print.h decode.h execute.h global.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/utils.o: utils.c utils.h global.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/print.o: print.c print.h utils.h global.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/decode.o: decode.c  decode.h state.h instruction.h global.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/execute.o: execute.c execute.h state.h instruction.h global.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(wildcard $(OBJ_DIR)/*.o)
	rm -f $(BUILD)


.SUFFIXES: .c .o .h
.PHONY: all clean