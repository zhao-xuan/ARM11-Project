/*
 * symbol interface: inserting labels and its address, retrieving label address
 */

#ifndef TABLE_H
#define TABLE_H

#include "global.h"
#include "linked_list.h"

typedef struct {
  char *label;
  void *literal;
} symbol_t;

typedef linked_list symbol_table_t;

symbol_table_t *create_table();

void free_table(symbol_table_t *table);

bool insert_symbol(symbol_table_t *table, symbol_t *symbol);

void *get_literal(symbol_table_t *table, char *label);
#endif
