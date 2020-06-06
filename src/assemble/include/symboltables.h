#include "instructions.h"
#include "table.h"

#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

typedef struct
{
	enum InstructionType type;
	byte_t cond;
	byte_t opcode;
} *parse_t;



symbol_table_t *init_parse_table();

void free_parse_table(symbol_table_t *parse_table);

enum InstructionType get_type(symbol_table_t *parse_table, char *mnemonic);

byte_t get_cond(symbol_table_t *parse_table, char *mnemonic);

byte_t get_opcode(symbol_table_t *parse_table, char *mnemonic);

symbol_table_t *init_label_table();

void free_label_table(symbol_table_t *label_table);

address_t get_label_address(symbol_table_t *label_table, char *mnemonic);

#endif