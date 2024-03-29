/* two-pass tables: labels and opcode */

#ifndef SYMBOL_H
#define SYMBOL_H

#include "symbol_table.h"
#include "tokenizer.h"

/*
 * @param aspg: a tokenised assembly program.
 * @returns: a table mapping labels to addesses.
 */
symbol_table_t *init_label_table(assembly_program *aspg);

/*
 * @param label_table: Table generated by init_label_table.
 * @param mnemonic: The label as a string.
 * @returns: a table mapping labels to addesses.
 */
address_t get_label_address(symbol_table_t *label_table, char *mnemonic);

#endif
