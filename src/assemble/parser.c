#include "parser.h"
// Declarations of static helper functions for the parser below:
static void parse_dp(assembly_line *line, word_t bin, machine_code *mcode);
// Declarations for string processing (helper) functions below:
static char **operand_processor(char *operand);
static char *trim_field(char *str);

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
/* DATA_PROCESSING parsing functions*/
static void parse_dp(assembly_line *line, word_t *bin) {
  byte_t opcode_field = (bin >> OPCODE_LOCATION) & FOUR_BIT_FIELD;
  char **tokens = operand_processor(line->operands);
  if (opcode_field <= 4 || opcode_field == 12) {
    /* result-computing instruction */
    bin = bin | ();
  } else if (opcode_field == 13) {
    /* mov instruction */
    
  } else if (opcode_field >= 8 && opcode_field <= 10) {
    /* CPSR flag set instruction */
  }
}

static wort_t parse_dp_operand2(char *operand2) {
  
}

static byte_t reg_to_bin(char *reg) {
  return atoi(reg + 1);
}

// Implementation for the string processing (helper) functions below:
/*
 * Second-pass: tokenize the operand field
 * @param: char *operand: the operand fields in a string
 * @return: an array of string representing the operand fields
 */

#define MAX_FIELD_COUNT 4;

static char **operand_processor(char *operand) {
  char **tokens = calloc(MAX_FIELD_COUNT, sizeof(char *));
  int i = 0;
  char *literal = strtok(operand, ",");
  while (literal != NULL) {
    tokens[i] = malloc(strlen(literal) * sizeof(char));
    strcpy(token[i], literal);
    i++;
    literal = strtok(NULL, ",");
  }
  tokens = realloc(tokens, i * sizeof(char *));

  return tokens;
}

/*
 * @brief: deleting any leading spaces, '[' and ']' in a string. This function would only deal with white spaces at the beginning of the string or '[' and ']'
 * @param: char *str: the string to be processed
 * @return: str without leading spaces
 */
static char *trim_field(char *str) {
  int i = 0;
  while (*(str + i) == ' ' || *(str + i) == '[') {
    i++;
  }
  char *trimmed = malloc((strlen(str) - i) * sizeof(char));
  strcpy(trimmed, str + i);
  if (*(str + strlen(str) - 1) == ']') {
    trimmed = realloc(trimmed, strlen(trimmed) - 1);
  }
  free(str);
  return trimmed;
}

