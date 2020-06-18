#include "label_table.h"

symbol_table_t *init_label_table(assembly_program *aspg) {
  symbol_table_t *symbol_table = create_table();
  for (int i = 0; i < aspg->total_lines; i++) {
    if (aspg->lines[i]->label != NULL) {
      address_t *location_counter = eMalloc(sizeof(address_t));
      *location_counter = aspg->lines[i]->location_counter;
      insert_literal(symbol_table, aspg->lines[i]->label, location_counter);
    }
  }
  return symbol_table;
}

address_t get_label_address(symbol_table_t *label_table, char *mnemonic) {
  address_t *address = get_literal(label_table, mnemonic);
  if (address == NULL) return -1;
  return *address;
}
