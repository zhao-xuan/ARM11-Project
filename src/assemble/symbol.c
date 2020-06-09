#include "symbol.h"

typedef struct {
  enum InstructionType type;
  byte_t cond;
  byte_t opcode;
} * parse_t;

symbol_table_t *init_parse_table() {
  symbol_table_t *symbol_table = create_table();
  // reads the tokens and inserts the required

  return symbol_table;
}

symbol_table_t *init_label_table(assembly_program aspg) {
  symbol_table_t *symbol_table = create_table();
  for (size_t i = 0; aspg[i]; i++) {
    if (aspg[i]->label != NULL) {
      address_t *location_counter = malloc(sizeof(address_t));
      *location_counter = aspg[i]->location_counter;
      insert_literal(symbol_table, strtok(aspg[i]->label, ":"),
                     location_counter);
    }
  }
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

address_t get_label_address(symbol_table_t *label_table, char *mnemonic) {
  address_t *address = get_literal(label_table, mnemonic);
  return *address;
}