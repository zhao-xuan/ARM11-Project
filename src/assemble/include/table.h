/*
 * symbol interface: inserting labels and its address, retrieving label address
 */

#ifndef TABLE_H
#define TABLE_H

#include "global.h"
#include "linked_list.h"

typedef struct {
  char *label;
  word_t address;
} symbol_t;

typedef linked_list symbol_table_t;

symbol_table_t *create_table();

void free_table(symbol_table_t *table);

bool find_symbol(symbol_table_t *table, symbol_t *symbol);

bool insert_symbol(symbol_table_t *table, symbol_t *symbol);

bool remove_symbol(symbol_table_t *table, symbol_t *symbol);


#endif
