#include <string.h>
#include "parser.h"

/* Translates a register/hash/equal expression into its numerical expression. */
#define to_index(literal, nptr) ((int) strtol(literal + 1, nptr, 0))

// Declarations of static helper functions for the parser below:

static void parse_mul(word_t *bin, char **operands, const char *mnemonic);
static void parse_b(word_t *bin, char **operands, symbol_table_t *label_table, address_t current);

// Declarations for string processing (helper) functions below:
static char **operand_processor(char *operand);
static char *trim_field(char *str);

// Implementation for public functions:
machine_code *parse(assembly_program *program, symbol_table_t *label_table) {
  machine_code *mcode = malloc(sizeof(machine_code));
  mcode->length = program->total_lines;
  mcode->bin = calloc(mcode->length, sizeof(word_t));

  for (int i = 0; i < program->total_lines; i++) {
    //assembly_line *line = program->lines[i];
    //mnemonic_p content = get_mnemonic_data(line->opcode);
    //word_t bin = content->bin;
  }
  // TODO
  return NULL;
}

void free_machine_code(machine_code *mcode) {
  free(mcode->bin);
  free(mcode);
}

// Implementation for the parser helper functions below:

/*
 * Parser for multiplication instructions
 * @param *bin: a pointer to the 32-bits binary word parsed by this function
 * @param **operands: an array of strings that holds the operands
 * @param *mnemonic: mnemonic of the multiplication instruction
 */
static void parse_mul(word_t *bin, char **operands, const char *mnemonic) {
  /* Sets Rd register */
  *bin |= (to_index(operands[0], NULL) & FOUR_BIT_FIELD) << 16;
  /* Sets Rn register */
  *bin |= (to_index(operands[1], NULL) & FOUR_BIT_FIELD) << 12;
  /* Sets Rs register */
  *bin |= (to_index(operands[2], NULL) & FOUR_BIT_FIELD) << 8;
  /* Sets Rm register */
  if (strcmp(mnemonic, "mla") == 0) {
    *bin |= (to_index(operands[3], NULL) & FOUR_BIT_FIELD);
  }
}

/*
 * Parser for branching instructions
 * @param *bin: a pointer to the 32-bits binary word parsed by this function
 * @param **operands: an array of strings that holds the operands
 * @param *label_table: a pointer to the label table produced in the first pass
 */
static void parse_b(word_t *bin, char **operands, symbol_table_t *label_table, address_t current) {
  char *errptr = NULL;
  word_t addr = (word_t) to_index(operands[0], errptr);
  /* Expression is a label, not a number */
  if (errptr != NULL) {
    addr = get_label_address(label_table, operands[0]);
  }

  *bin |= ((addr - current - 8) >> 2) & TWENTY_FOUR_BIT_FIELD;
}

// Implementation for the string processing (helper) functions below:
/*
 * Second-pass: tokenize the operand field
 * @param: char *operand: the operand fields in a string
 * @return: an array of string representing the operand fields
 */

#define MAX_FIELD_COUNT 4

static char **operand_processor(char *operand) {
  char **tokens = calloc(MAX_FIELD_COUNT, sizeof(char *));
  int i = 0;
  char *literal = strtok(operand, ",");
  while (literal != NULL) {
    tokens[i] = malloc(strlen(literal) * sizeof(char));
    strcpy(tokens[i], literal);
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

