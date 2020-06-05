/*
 * Interfaces for the utility functions
 */

#ifndef UTILS_H
#define UTILS_H

#include "global.h"
#include <stddef.h>
#include <stdint.h>
/*
 * @brief: Loads a binary file into the buffer.
 * @param *path: path to the binary file.
 * @param **buffer: a pointer to the buffer that will load the binary file.
 * @param *size: a pointer to the size of the file (length of buffer).
 * @example:
 *   uint32_t *buffer = NULL;
 *   size_t size;
 *   if (read_binary_file(argv[1], &buffer, &size) == 0)
 *   {
 *     printf("%ld\n", size);
 *     dump_hex(buffer, size);
 *     free(buffer);
 *   }
 */
void read_binary_file(const char *path, uint32_t **buffer, size_t *size);

/*
 * @brief: Loads a binary file into the buffer.
 * @param path: path to the binary file.
 * @param buffer: a pointer to the buffer that will load the binary file.
 * @param size: a pointer to the size of the file (length of buffer).
 */
void write_binary_file(const char *path, uint32_t *buffer, size_t size);

/*
 * @brief: Loads an assembly file into the buffer.
 * @param path: path to the assembly file.
 * @returns : A pointer to a string array on the heap, NULL terminated.
 * @example:
 *    char **buffer = read_assembly_file(argv[1]);
 *      for (int i = 0; buffer[i]; i++) {
 *      printf("%s\n",buffer[i]); 
 *      free(buffer[i]);
 *     }
 *    free(buffer);
 */
char **read_assembly_file(const char *path);

/*
 * @brief: takes in an unsigned 32 bit number, and print out its bits to stdout.
 * @param x: the number to be printed.
 */
void print_bits(uint32_t x);

/*
 * @brief: takes in an array of 32 bit numbers, and print out its bits to
 * stdout.
 * @param *buffer: a pointer to the buffer to be dumped.
 * @param size: the size of the buffer.
 */
void dump_buffer(uint32_t *buffer, size_t size);

/*
 * @brief: takes in an array of 32 bit numbers, and print out its hex form to
 * stdout.
 * @param *buffer: a pointer to the buffer to be dumped.
 * @param size: the size of the buffer.
 */
void dump_hex(uint32_t *buffer, size_t size);

/*
 * @brief: Sign extend a number to 32 bits.
 * @param x: The number to sign extend.
 * @param bits: the number of bits in x.
 */
uint32_t sign_extend(uint32_t x, int bits);
#endif
