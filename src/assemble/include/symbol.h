/*
 * two-pass tables: labels and opcode
 */

#ifndef SYMBOL_H
#define SYMBOL_H

#include "instructions.h"
#include "table.h"

symbol_table_t *init_parse_table();

enum InstructionType get_type(symbol_table_t *parse_table, char *mnemonic);

byte_t get_cond(symbol_table_t *parse_table, char *mnemonic);

byte_t get_opcode(symbol_table_t *parse_table, char *mnemonic);

symbol_table_t *init_label_table();

address_t get_label_address(symbol_table_t *label_table, char *mnemonic);

#endif