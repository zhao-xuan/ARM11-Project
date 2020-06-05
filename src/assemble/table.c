#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "linked_list.h"
#include "table.h"
#include "global.h"


int symbol_comparator(void *this, void *that) {
  return strcmp(((symbol_t *)this)->label, 
                ((symbol_t *)that)->label);
}

symbol_table_t *create_table() {
  return create_linked_list(&symbol_comparator);
}

void free_table(symbol_table_t *table){
  free_list(table);
}

bool find_symbol(symbol_table_t *table, symbol_t *symbol) {
  return find(table, (void *) symbol, sizeof(*symbol)); 
}

bool insert_symbol(symbol_table_t *table, symbol_t *symbol) {
  return push(table, (void *) symbol, sizeof(*symbol));
}

bool remove_symbol(symbol_table_t *table, symbol_t *symbol) {
  return pop(table, (void *) symbol, sizeof(*symbol));
}

word_t get_address(symbol_table_t *table, char *label) {
  symbol_t partial_symbol = (symbol_t) {label};
  symbol_t *symbol_p = (symbol_t *) get(table, (void *) &partial_symbol, sizeof(*symbol_p));
  
  if (symbol_p) {
    return symbol_p->address;
  } else {
    fprintf(stderr, "Symbol cannot be found, exiting...\n");
    return 0;
  }
}

