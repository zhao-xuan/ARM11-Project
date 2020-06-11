#include <string.h>
#include "parser.h"
#include "exceptions.h"

/* Translates a register/hash/equal expression into its numerical expression. */
#define to_index(literal) ((int) strtol(literal + 1, NULL, 0))
#define equal(literal) (literal[0] == '=')
#define hash(literal) (literal[0] == '#')

// Declarations of static helper functions for the parser below:

static void parse_mul(word_t *bin, char **operands, const char *mnemonic);
static void parse_dt(word_t *bin, char **operands, word_t *data, address_t offset);
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
    assembly_line *line = program->lines[i];
    char **operands = operand_processor(line->operands);
    mnemonic_p content = get_mnemonic_data(line->opcode);

    /* Special case for ldr interpreted as mov */
    if ((strcmp(line->opcode, "ldr")) && (operands[2] != NULL) &&
         equal(operands[2]) && (to_index(operands[2]) <= 0xFF)) {
      content = get_mnemonic_data("mov");
    }
    word_t bin = content->bin;
    word_t data;
    address_t current = i * 4;


    switch (content->type) {
    case DATA_PROCESSING:
      break;
    case MULTIPLY:
      parse_mul(&bin, operands, line->opcode);  
      break;
    case DATA_TRANSFER:
      /* Calculates the offset and consider the effect of the pipeline */
      parse_dt(&bin, operands, &data, current - line->location_counter - 12);
      if (data != 0) { /* Append data to the end of the machine code */
        mcode->length++;
        mcode->bin = realloc(mcode->length, sizeof(word_t));
        mcode->bin[mcode->length - 1] = data;
      }
    case BRANCH:
      parse_b(&bin, operands, label_table, current);
      break;
    case HALT:
      break;  
    default:
      exceptions(UNKNOWN_INSTRUCTION_TYPE, current);
    }
  }
  return mcode;
}

void free_machine_code(machine_code *mcode) {
  free(mcode->bin);
  free(mcode);
}

// Implementation for the parser helper functions below:
/* constant-express
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
    char **pre = operand_processor(trim_field(pre), 2);
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

