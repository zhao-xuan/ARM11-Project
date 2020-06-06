#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "linked_list.h"
#include "table.h"
#include "global.h"


static int symbol_comparator(void *this, void *that) {
  return strcmp((char *) this, (char *) that);
}

symbol_table_t *create_table() {
  return create_linked_list(&symbol_comparator);
}

void free_table(symbol_table_t *table){
  free_list(table);
}

bool insert_literal(symbol_table_t *table, char *label, void *literal) {
  return insert(table, label, strlen(label) + 1, literal);
}

bool insert_symbol(symbol_table_t *table, symbol_t *symbol) {
  return insert_literal(table, symbol->label, symbol->literal);
}

void *get_literal(symbol_table_t *table, char *label) {
  return find(table, label); 
}

