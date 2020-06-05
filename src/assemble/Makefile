PROJ_ROOT      = ../..
BIN_DIR        = $(PROJ_ROOT)/bin
LIB_DIR        = $(PROJ_ROOT)/lib
OBJ_DIR        = $(PROJ_ROOT)/obj/assemble
COMMON_OBJ_DIR = $(PROJ_ROOT)/obj/common
SRC_FILES      = $(wildcard *.c)
OBJ_FILES      = $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))


CC          = gcc
IncludePath = -Iinclude $(addprefix -I, $(wildcard $(LIB_DIR)/*/.)) $(addprefix -I, $(wildcard ../common/*/.))
CFLAGS      = -O $(IncludePath) -Wall -g -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
LDFLAGS     = $(addprefix -L, $(wildcard $(LIB_DIR)/*/.))
BUILD       = libs $(BIN_DIR)/assemble

all: $(BUILD)

$(BIN_DIR)/assemble: $(OBJ_FILES) $(COMMON_OBJ_DIR)/utils.o
	$(CC) $^ $(LDFLAGS) $(wildcard $(LIB_DIR)/*/*.a) -o $@

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(COMMON_OBJ_DIR)/utils.o: ../common/utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

libs:
	cd $(LIB_DIR); make

clean:
	rm -f $(wildcard $(OBJ_DIR)/*.o)
	rm -f $(wildcard $(UTIL_OBJ_DIR)/*.o)
	rm -f $(BIN_DIR)/assemble
	cd tests; make clean
	cd $(LIB_DIR); make clean

.SUFFIXES: .c .o .h
.PHONY: clean test libs
