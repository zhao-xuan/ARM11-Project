#include <string.h>
#include "parser.h"

/* Translates a register/hash/equal expression into its numerical expression. */
#define to_index(literal) ((int) strtol(literal + 1, NULL, 0))
#define equal(literal) (literal[0] == '=')
#define hash(literal) (literal[0] == '#')

// Declarations of static helper functions for the parser below:
static void parse_dp(assembly_line *line, word_t *bin);
static word_t parse_dp_operand2(char *operand2);
static void parse_mul(word_t *bin, char **operands, const char *mnemonic);
static void parse_dt(word_t *bin, char **operands, word_t *data, address_t offset);
static void parse_b(word_t *bin, char **operands, symbol_table_t *label_table, address_t current);

// Declarations for string processing (helper) functions below:
static char **operand_processor(const char *operand, int field_count);
static char *trim_field(char *str);
#define to_index(literal) ((int) strtol(literal + 1, NULL, 0))
// Implementation for public functions:
machine_code *parse(assembly_program *program, symbol_table_t *label_table) {
  machine_code *mcode = malloc(sizeof(machine_code));
  mcode->length = program->total_lines;
  mcode->bin = calloc(mcode->length, sizeof(word_t));

  /*for (int i = 0; i < program->total_lines; i++) {
    assembly_line *line = program->lines[i];
    mnemonic_p content = get_mnemonic_data(line->opcode);
    word_t bin = content->bin;
  }*/
  // TODO
  return NULL;
}

void free_machine_code(machine_code *mcode) {
  free(mcode->bin);
  free(mcode);
}

// Implementation for the parser helper functions below:
/* DATA_PROCESSING parsing functions*/

static void parse_dp(assembly_line *line, word_t *bin) {
  byte_t opcode_field = (*bin >> OPCODE_LOCATION) & FOUR_BIT_FIELD;
  char **tokens = operand_processor(line->operands, 3);
  if (opcode_field <= 4 || opcode_field == 12) {
    /* result-computing instruction */
    *bin |= to_index(tokens[0]) << DP_DT_RD_LOCATION;
    *bin |= to_index(tokens[1]) << DP_DT_RN_LOCATION;
    *bin |= parse_dp_operand2(tokens[2]); //parse_dp_operand2 will take care of the immediate bit as well
  } else if (opcode_field == 13) {
    /* mov instruction */
    *bin |= to_index(tokens[0]) << DP_DT_RD_LOCATION;
    *bin |= parse_dp_operand2(tokens[1]); //parse_dp_operand2 will take care of the immediate bit as well
  } else if (opcode_field >= 8 && opcode_field <= 10) {
    /* CPSR flag set instruction */
    *bin |= to_index(tokens[0]) << DP_DT_RN_LOCATION;
    *bin |= parse_dp_operand2(tokens[1]); //parse_dp_operand2 will take care of the immediate bit as well
  }
}

static word_t parse_dp_operand2(char *operand2) {
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
      if (shamt < 32) {
        bin |= shamt << OPERAND2_INTEGER_SHIFT_LOCATION;
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

/*
 * Parser for multiplication instructions
 * @param *bin: a pointer to the 32-bits binary word parsed by this function
 * This function sets bits [19-8], [3-0] of *bin
 * @param **operands: an array of strings that holds the operands
 * @param *mnemonic: mnemonic of the multiplication instruction
 */
static void parse_mul(word_t *bin, char **operands, const char *mnemonic) {
  /* Sets Rd register */
  *bin |= (to_index(operands[0]) & FOUR_BIT_FIELD) << 16;
  /* Sets Rn register */
  *bin |= (to_index(operands[1]) & FOUR_BIT_FIELD) << 12;
  /* Sets Rs register */
  *bin |= (to_index(operands[2]) & FOUR_BIT_FIELD) << 8;
  /* Sets Rm register */
  if (strcmp(mnemonic, "mla") == 0) {
    *bin |= (to_index(operands[3]) & FOUR_BIT_FIELD);
  }
}

/*
 * Parser for single data transfer instructions
 * @param *bin: a pointer to the 32-bits binary word parsed by this function
 * This function sets bits [25-23], [19-0] of *bin
 * @param **operands: an array of strings that holds the operands
 * @param *data: a pointer to the data that need to be appended at the end of the machine code.
 * *data will be set to 0 if no data need to be appended. Note if data need to be appended,
 * it can't be 0, since such instructions will be interpreted as a mov instruction
 * @param offset: the offset of the current address to the end of the memory, taking into
 * account of the pipeline
 */
static void parse_dt(word_t *bin, char **operands, word_t *data, address_t offset) {
  *data = 0;
  /* Sets Rd Register */
  *bin |= (to_index(operands[0]) & FOUR_BIT_FIELD) << 12;
  /* Pre-indexing if no comma can be found in the second operand OR there are only two operands */
  bool pre_index = strchr(operands[1], ',') != NULL || operands[2] == NULL;
  bool up = true;

  if (equal(operands[1])) {  /* Load value (equal expression) */
    *data = to_index(operands[1]);
    /* Sets Rn to PC */
    *bin |= PC << 16;
    *bin |= offset & TWELVE_BIT_FIELD;
  } else if (pre_index) { /* Pre-indexing */
    char **pre = operand_processor(trim_field(pre, 2));
    *bin |= (to_index(pre[0]) & FOUR_BIT_FIELD) << 16;

    if (pre[1] != NULL) {
      if (hash(pre[1])) { /* Hash expression */
        *bin |= to_index(pre[1]) & TWELVE_BIT_FIELD;
      } else { /* Operand2 */
        up = !(pre[1][0] == '-');
        *bin |= parse_operand2(operands[2]);
      }
    }
  } else { /* Post-indexing */
    /* Sets Rn Register */
    *bin |= (to_index(trim_field(operands[1])) & FOUR_BIT_FIELD) << 16;
    if (hash(operands[2])) { /* Hash expression */
      *bin |= to_index(operands[2]) & TWELVE_BIT_FIELD;
    } else { /* Operand2 */
      up = !(operands[1][0] == '-');
      *bin |= parse_operand2(operands[2]);
    }
  }

  *bin |= (pre_index & 1) << 24;
  *bin |= (up & 1) << 23;
}


/*
 * Parser for branching instructions
 * @param *bin: a pointer to the 32-bits binary word parsed by this function
 * This function sets bits [24-0] of *bin
 * @param **operands: an array of strings that holds the operands
 * @param *label_table: a pointer to the label table produced in the first pass
 */
static void parse_b(word_t *bin, char **operands, symbol_table_t *label_table, address_t current) {
  char *errptr = NULL;
  word_t addr = (word_t) strtol(operands[0] + 1, &errptr, 0);
  /* Expression is a label, not a number. See documentation for strtol */
  if (errptr != NULL) {
    addr = get_label_address(label_table, operands[0]);
  }

  *bin |= ((addr - current - 8) >> 2) & TWENTY_FOUR_BIT_FIELD;
}

// Implementation for the string processing (helper) functions below:
/*
 * Second-pass: tokenize the operand field
 * @param: char *operand: the operand fields in a string
 * @param: int field_count: the maximum number of tokens(fields). This is needed in situations where <operand2> in DATA_PROCESSING and <address> in DATA_TRANSFER should not be further tokenized
 * @return: an array of string representing the operand fields
 */

static char **operand_processor(const char *operand, int field_count) {
    char **tokens = calloc(field_count, sizeof(char *));
    int i = 0;
    char str[strlen(operand) + 1];
    for (int j = 0; j < strlen(operand); j++) {
        str[j] = operand[j];
    }
    str[strlen(operand)] = '\0';
    char *literal = strtok(str, ",");
    while (literal != NULL && i < field_count) {
        tokens[i] = malloc(strlen(literal) * sizeof(char));
        strcpy(tokens[i], literal);
        i++;
        literal = strtok(NULL, ",");
    }

    tokens = realloc(tokens, (i + 1) * sizeof(char *));
    tokens[i] = NULL;

    return tokens;
}

/*
 * @brief: free an array of string tokens
 * @param: char **tokens: the array of string tokens that need to be freed
 */
static void free_tokens(char **tokens) {
  int i = 0;
  while (tokens[i] != NULL) {
    free(tokens[i]);
    i++;
  }

  free(tokens);
}

/*
 * @brief: deleting any leading spaces, '[' and ']' in a string. This function would only deal with white spaces at the beginning of the string or '[' and ']'
 * @param: char *str: the string to be processed
 * @return: str without leading spaces
 */
static char *trim_field(char *str) {
    int i = 0;
    while (str[i] == ' ' || str[i] == '[') {
        i++;
    }
    int j = strlen(str) - 1;
    while (!(str[j] - '0' >= 0 && str[j] - '0' <= 9) && !(str[j] - 'a' >= 0 && str[j] - 'a' <= 25)) {
        j--;
    }
    char *trimmed = malloc((j - i) * sizeof(char));
    strncpy(trimmed, str + i, j - i + 1);

    return trimmed;
}
