/*
  1. Might be useful for I/O operations.
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 *  @brief: pass by value-result. REMEMBER TO FREE buffer when done.
 *  Takes in path to a file, returns content of the file, and size of the file.
 *  Remember to swap endianness when loading memory. 
 *  @param:
 *    *path: path to the required file.
 *    **buffer: an empty pointer, will point to the content of the file.
 *    *size: an empty pointer, the value will be set to the size of the file.
 *  @example:
 *    uint32_t *buffer = NULL;
 *    size_t size;
 *    if (read_binary_file(argv[1], &buffer, &size) == 0) {
 *      printf("%ld\n", size);
 *      dump_hex(buffer, size);
 *      free(buffer);
 *    }
 */
void read_binary_file(const char *path, uint32_t **buffer, size_t *size);

/*
 *  @brief: takes in an unsigned 32 bit number, and print out its bits.
 *  @param:
 *    x: the number to be printed.
 */
void print_bits(uint32_t x);

/*
 *  @brief: takes in an array of 32 bit numbers, and print out its bits.
 *  @param:
 *    buffer: the buffer to be dumped.
 *    size: the size of the buffer.
 */
void dump_buffer(uint32_t *buffer, size_t size);


/*
 *  @brief: takes in an array of 32 bit numbers, and print out its hex form.
 *  @param:
 *    buffer: the buffer to be dumped.
 *    size: the size of the buffer.
 */
void dump_hex(uint32_t *buffer, size_t size);


/*
 *  @brief: checks whether a given value is out of bounds. 
 *  @param:
 *    addr: the position to be verified. 
 *    size: the size of the total array. 
 */
int out_of_bound_check(uint32_t addr, size_t size);

/*
 *  @brief: Sign extend a number to 32 bits. 
 *  @param:
 *    x: The number to sign extend. 
 *    bits: the number of bits in x. 
 */
uint32_t sign_extend(uint32_t x, int bits);
#endif
