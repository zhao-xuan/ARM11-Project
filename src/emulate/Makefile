PROJ        = ../..
BIN_DIR     = $(PROJ)/bin
LIB_DIR     = $(PROJ)/lib
OBJ_DIR     = $(PROJ)/obj/emulate
COMMON_OBJ  = $(PROJ)/obj/common
SRC_FILES   = $(wildcard *.c)
OBJ_FILES   = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEPS = $(OBJ_FILES:%.o=%.d) $(COMMON_OBJ)/utils.d

CC          = gcc
IncludePath = -Iinclude -I../common/include
CFLAGS      = -MMD -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic $(IncludePath)
LFLAGS      = 

all: libs $(BIN_DIR)/emulate

$(BIN_DIR)/emulate: $(OBJ_FILES) $(COMMON_OBJ)/utils.o
	$(CC) $^ $(LFLAGS) -o $@

libs:
	cd $(LIB_DIR); make

clean:
	rm -f $(DEPS)
	rm -f $(wildcard $(OBJ_DIR)/*.o)
	rm -f $(wildcard $(COMMON_OBJ)/*.o)
	rm -f $(BIN_DIR)/emulate
	cd tests; make clean
	cd $(LIB_DIR); make clean

# pre-processor automatically adds headers as dependencies
-include $(DEPS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(COMMON_OBJ)/%.o: ../common/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.SUFFIXES: .c .o .h
.PHONY: clean test libs