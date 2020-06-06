/*
 * Four assembling functions
 */

#ifndef PARSE_H
#define PARSE_H

#include "tokenizer.h"
#include "instructions.h"

/*
 * Parse the tokenized inputs into instruction structures
 * @param line: The tokenized input line.
 * @return: The instruction parsed into.
 */
instruction_t *parse(token_line_t *line);

#endif