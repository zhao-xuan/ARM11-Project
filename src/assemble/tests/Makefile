# This Makefile is for testing purpose ONLY
PROJ_ROOT   = ../../..
LIB_DIR     = $(PROJ_ROOT)/lib
COMMON_DIR  = $(PROJ_ROOT)/src/common

TEST_FILES  = $(wildcard *_test.c)
BUILD       = $(patsubst %_test.c, %_test, $(TEST_FILES))

SRC_FILES   = $(wildcard ../*.c)
SRC_NO_MAIN = $(filter-out ../assemble.c, $(SRC_FILES))
OBJ_FILES   = $(patsubst ../%.c, %.o,$(SRC_NO_MAIN)) utils.o

CC          = gcc
IncludePath = -I../include $(addprefix -I, $(wildcard $(LIB_DIR)/*/.)) $(addprefix -I, $(wildcard $(COMMON_DIR)/*/.))
CFLAGS      = -g -Wall $(IncludePath) -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic
LDFLAGS     = $(addprefix -L, $(wildcard $(LIB_DIR)/*/.))

all: lib $(BUILD)

%_test: %_test.o $(OBJ_FILES)
	$(CC) $^ $(LDFLAGS) $(wildcard $(LIB_DIR)/*/*.a) -o $@

%_test.o: %_test.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: ../%.c
	$(CC) $(CFLAGS) -c -o $@ $<

utils.o: $(COMMON_DIR)/utils.c
	$(CC) $(CFLAGS) -c -o $@ $<

lib:
	cd $(LIB_DIR); make

clean:
	rm -f $(BUILD) *.o core
	cd $(LIB_DIR); make clean	

.PHONY: clean, testutils