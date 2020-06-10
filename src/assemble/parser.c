#include "parser.h"
// Declarations of static helper functions for the parser below:

// Declarations for string processing (helper) functions below:

// Implementation for public functions:
machine_code *parse(assembly_program *program, symbol_table_t *label_table) {
  // TODO
  return NULL;
}

void free_machine_code(machine_code *mcode) {
  free(mcode->bin);
  free(mcode);
}

// Implementation for the parser helper functions below:

// Implementation for the string processing (helper) functions below:
