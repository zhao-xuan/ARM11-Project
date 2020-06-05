CC = gcc
CFLAGS = -Wall -Werror -Iinclude 
LIB = libADT.a
LIBOBJS = linked_list.o
BUILD = $(LIB)

all: $(BUILD)

$(LIB): $(LIBOBJS)
	ar rcs $(LIB) $(LIBOBJS)

clean:
	rm -f $(BUILD) *.o core

linked_list.o: include/linked_list.h
