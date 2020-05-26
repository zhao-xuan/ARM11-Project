/* 
	1. Might be useful for I/O operations. 
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 *@brief: pass by value-result. REMEMBER TO FREE buffer when done. 
 *Takes in path to a file, returns content of the file, and size of the file. 
 *	@param: 
 *		*path: path to the required file. 
 *		**buffer: an empty pointer, will point to the content of the file when done. 
 *		*size: an empty pointer, the value will be set to the size of the file when done. 
 *	@returns: -1 if an error occurred, and 0 if OK. 
 *	@example: 
 *		uint32_t *buffer = NULL;
 *		int size;
 *		if (read_binary_file("./hello", &buffer, &size) == 0) {
 *			printf("%d\n", size);
 *			dump_buffer(buffer, size);
 *			free(buffer);
 *	}
 */
int read_binary_file(char *path, uint32_t **buffer, int *size);


/*
 *	@brief: takes in an unsigned 32 bit number, and print out its bits. 
 *	@param: 
 *		x: the number to be printed. 
 */
void print_bits(uint32_t x);



/*
 *	@brief: takes in an array of 32 bit numbers, and print out its bits. 
 *	@param: 
 *		buffer: the buffer to be dumped. 
 *		size: the size of the buffer. 
 */
void dump_buffer(uint32_t *buffer, int size);


#endif
