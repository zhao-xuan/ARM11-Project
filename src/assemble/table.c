#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "linked_list.h"
#include "table.h"
#include "global.h"


int symbol_comparator(symbol_t *this, symbol_t *that) {
  return strcmp(this->label, that->label);
}

symbol_table_t *create_table() {
  return create_linked_list(&symbol_comparator);
}

void free_table(symbol_table_t *table){
  free_list(table);
}

bool find_symbol(symbol_table_t *table, symbol_t *symbol) {
  return find(table, (void *) symbol, sizeof(symbol)); 
}

bool insert_symbol(symbol_table_t *table, symbol_t *symbol) {
  return push(table, (void *) symbol, sizeof(symbol));
}

bool remove_symbol(symbol_table_t *table, symbol_t *symbol) {
  return pop(table, (void *) symbol, sizeof(symbol));
}




