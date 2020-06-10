#include "parser.h"
// Declarations of static helper functions for the parser below:

// Declarations for string processing (helper) functions below:

// Implementation for public functions:
machine_code *parse(assembly_program *program, symbol_table_t *label_table) {
  machine_code *mcode = malloc(sizeof(machine_code));
  mcode->length = program->total_lines;
  mcode->bin = calloc(mcode->length, sizeof(word_t));

  for (int i = 0; i < program->total_lines; i++) {
    assembly_line *line = program->lines[i];
    mnemonic_p content = get_mnemonic_data(line->opcode);
    word_t bin = content->bin;
    
  }
  // TODO
  return NULL;
}

void free_machine_code(machine_code *mcode) {
  free(mcode -> bin);
  free(mcode);
}

// Implementation for the parser helper functions below:

// Implementation for the string processing (helper) functions below:
