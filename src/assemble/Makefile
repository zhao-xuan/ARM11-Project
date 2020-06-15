PROJ        = ../..
BIN_DIR     = $(PROJ)/bin
LIB_DIR     = $(PROJ)/lib
OBJ_DIR     = $(PROJ)/obj/assemble
COMMON_OBJ  = $(PROJ)/obj/common
SRC_FILES   = $(wildcard *.c)
OBJ_FILES   = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
LIB_HEADERS = $(addprefix -I, $(wildcard $(LIB_DIR)/*/.)) $(addprefix -I, $(wildcard $(LIB_DIR)/*/include))

CC          = gcc
IncludePath = -Iinclude -I../common/include $(LIB_HEADERS)
CFLAGS      = -Wall -g -D_DEFAULT_SOURCE -std=c99 $(IncludePath)
LFLAGS      = $(addprefix -L, $(wildcard $(LIB_DIR)/*/.)) $(wildcard $(LIB_DIR)/*/*.a)

all: libs $(BIN_DIR)/assemble

$(BIN_DIR)/assemble: $(OBJ_FILES) $(COMMON_OBJ)/utils.o
	$(CC) $^ $(LFLAGS) -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(COMMON_OBJ)/%.o: ../common/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

libs:
	cd $(LIB_DIR); make

clean:
	rm -f $(wildcard $(OBJ_DIR)/*.o)
	rm -f $(wildcard $(COMMON_OBJ)/*.o)
	rm -f $(BIN_DIR)/assemble
	cd tests; make clean
	cd $(LIB_DIR); make clean

.SUFFIXES: .c .o .h
.PHONY: clean test libs
