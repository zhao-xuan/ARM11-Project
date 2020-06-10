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
  byte_t opcode_field = (*bin >> OPCODE_LOCATION) & FOUR_BIT_FIELD;
  char **tokens = operand_processor(line->operands);
  if (opcode_field <= 4 || opcode_field == 12) {
    /* result-computing instruction */
    *bin |= to_index(tokens[0]) << DP_DT_RD_LOCATION;
    *bin |= to_index(tokens[1]) << DP_DT_RN_LOCATION;
    *bin |= parse_dp_operand2(tokens[2]); //parse_dp_operand2 will take care of the immediate bit as well
  } else if (opcode_field == 13) {
    /* mov instruction */
    *bin |= to_index(tokens[0]) << DP_DT_RD_LOCATION;
    *bin |= parse_dp_operand2(tokens[1]); //parse_dp_operand2 will take care of     the immediate bit as well
  } else if (opcode_field >= 8 && opcode_field <= 10) {
    /* CPSR flag set instruction */
    *bin |= to_index(tokens[0]) << DP_DT_RN_LOCATION;
    *bin |= parse_dp_operand2(tokens[1]); //parse_dp_operand2 will take care of     the immediate bit as well
  }
}

static wort_t parse_dp_operand2(char *operand2) {
  word_t bin = 0;
  if (operand2[0] == '#') {
    bin |= 1 << IMM_LOCATION;
    long imm = to_index(operand2);
    if (imm >= 0 && imm < 128)  {
      bin |= imm;
    } else {
      /* where the right rotation is needed */
    }
  } else {
    char *base_reg = strtok(operand2, ",");
    bin |= to_index(base_reg);
    char *shift_name = strtok(NULL, " ");
    char *shift_name_list[] = {"lsl", "lsr", "asr", "ror"};
    for (int i = 0; i < 4; i++) {
      if (strcmp(shift_name_list[i], shift_name)) {
          bin |= i << OPERAND2_SHIFT_TYPE_LOCATION;
          break;
      }
    }

    char *shamt_str = strtok(NULL, " ");
    long shamt = to_index(shamt_str);
    if (shamt_str[0] == '#') {
      if (shift_integer < 32) {
        bin |= shift_integer << OPERAND2_INTEGER_SHIFT_LOCATION;
      } else {
        /* An error should be thrown: shift integer is not in the range */
      }
    } else {
      bin |= 1 << OPERAND2_SHIFT_SPEC_LOCATION;
      bin |= shamt << OPERAND2_REGISTER_SHIFT_LOCATION;
    }
  }

  return bin;
}

// Implementation for the string processing (helper) functions below:
/*
 * Second-pass: tokenize the operand field
 * @param: char *operand: the operand fields in a string
 * @return: an array of string representing the operand fields
 */

#define MAX_OPERAND2_FIELD_COUNT 3;

static char **operand_processor(char *operand) {
  char **tokens = calloc(MAX_OPERAND2_FIELD_COUNT, sizeof(char *));
  int i = 0;
  char *literal = strtok(operand, ",");
  while (literal != NULL || i < MAX_OPERAND2_FIELD_COUNT) {
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

