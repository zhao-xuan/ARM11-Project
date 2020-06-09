/*
 * tokenize the input lines, need to figure out how to store the tokens
 */

#ifndef TOKEN_H
#define TOKEN_H

#define MAX_LINE_LENGTH 511

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "global.h"

typedef struct {
  char *label;
  char *opcode;
  char *operands;
  address_t location_counter;
} *assembly_line;

/* NULL terminated array of assembly lines */
typedef struct {
  assembly_line *lines;
  size_t total_lines;
} *assembly_program;

assembly_program tokenize(char **buffer, size_t max_line);

size_t split_line(char *line, char **words);

#endif