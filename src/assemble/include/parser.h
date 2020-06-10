#ifndef PARSER_H
#define PERSER_H

#include <stdlib.h>
#include "label_table.h"
#include "mnemonic_table.h"
#include "tokenizer.h"


typedef struct {
  word_t *bin;
  size_t length;
} machine_code;


/*
 * Parse the tokenized assembly program into machine code
 * @param program: The tokenized assembly program
 * @param label_table: The label table for the assembly program
 * @returns: The ARM11 machine code of the assembly program
 */
machine_code *parse(assembly_program *program, symbol_table_t *label_table);

/*
 * Free the memory allocated for the machine code structure
 * @param binary: A pointer to the machine code structure to be freed
 */
void free_machine_code(machine_code *mcode);

#endif