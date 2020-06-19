/*
 * tokenize the input lines, need to figure out how to store the tokens
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "utils.h"

typedef struct {
  char *label;
  char *opcode;
  char *operands;
  address_t location_counter;
} assembly_line;

typedef struct {
  assembly_line **lines;
  size_t total_lines;
} assembly_program;

/*
 * @param buffer: buffer containing the raw input from read file.
 * @param max_line: The number of lines of the input file, an upper bound for
 * total_lines.
 * @returns: a structure containing an array of assembly lines broken into
 * label, opcode, and operands.
 */
assembly_program *tokenize(char **buffer, size_t max_line);

/* @param aspg: assembly_program to free. */
void free_program(assembly_program *aspg);
#endif
