#include "symboltables.h"

symbol_table_t *init_parse_table() {
  symbol_table_t *symbol_table = create_table();
	//reads the tokens and inserts the required 
  return symbol_table;
}

enum InstructionType get_type(symbol_table_t *parse_table, char *mnemonic) {
  parse_t parsed_node = get_literal(parse_table, mnemonic);
  return parsed_node->type;
}

byte_t get_cond(symbol_table_t *parse_table, char *mnemonic) {
  parse_t parsed_node = get_literal(parse_table, mnemonic);
  return parsed_node->cond;
}

byte_t get_opcode(symbol_table_t *parse_table, char *mnemonic) {
  parse_t parsed_node = get_literal(parse_table, mnemonic);
  return parsed_node->opcode;
}


symbol_table_t *init_label_table() {
    symbol_table_t *symbol_table = create_table();
	//reads the labels and inserts the required 
  return symbol_table;
}

address_t get_label_address(symbol_table_t *label_table, char *mnemonic) {
  address_t *address = get_literal(label_table, mnemonic);
    return *address;
}