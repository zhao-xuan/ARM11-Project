#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "exceptions.h"

#define to_index(literal) ((int) strtol(literal + 1, NULL, 0))
#define equal(literal) (literal[0] == '=')
#define hash(literal) (literal[0] == '#')
/* result-computing data processing instructions */
#define compute(opcode) (opcode_field <= 4 || opcode_field == 12)
#define move(opcode) (opcode_field == 13)
#define flag_set(opcode) (opcode_field >= 8 && opcode_field <= 10)

#define remove_bracket(literal) (strtok(literal + 1, "]"))
#define remove_space(string) for(; isspace(*string); string++)
#define open_brak(literal) (literal[0] == '[')
#define close_brak(literal) (literal[strlen(literal) - 1] == ']')

static void parse_dp(char **operands, word_t *bin);
static void parse_mul(word_t *bin, char **operands, const char *mnemonic);
static void parse_dt(word_t *bin, char **operands, word_t *data, address_t offset);
static void parse_b(word_t *bin, char **operands, symbol_table_t *label_table, address_t current);
static word_t parse_operand2(char *operand2);

static char **operand_processor(const char *operand, int field_count);
static void free_operands(char **tokens);

machine_code *parse(assembly_program *program, symbol_table_t *label_table) {
  machine_code *mcode = malloc(sizeof(machine_code));
  mcode->length = program->total_lines;
  mcode->bin = calloc(mcode->length, sizeof(word_t));

  for (int i = 0; i < program->total_lines; i++) {
    assembly_line *line = program->lines[i];
    char **operands = operand_processor(line->operands, 3);
    mnemonic_p content = get_mnemonic_data(line->opcode);

    /* Special case for ldr interpreted as mov */
    if ((strcmp(line->opcode, "ldr") == 0) && (operands[1] != NULL) &&
         equal(operands[1]) && (to_index(operands[1]) <= 0xFF)) {
      content = get_mnemonic_data("mov");
      /* Replace equal by hash */
      *strchr(line->operands, '=') = '#';
      strcpy(line->opcode, "mov");
    }
    free_operands(operands);

    /* Special case for lsl */
    if (strcmp(line->opcode, "lsl") == 0) {
      content = get_mnemonic_data("mov");
      char *rn = strtok(line->operands, ",");
      
      char expr[strlen(rn) * 2 + 6];
      strcpy(expr, rn);
      strcat(expr, ",lsl ");     
      strcat(expr, strtok(NULL, ""));      

      operands = calloc(3, sizeof(char *));
      operands[0] = rn;
      operands[1] = expr;

      word_t bin = content->bin;
      parse_dp(operands, &bin);
      mcode->bin[i] = bin;

      free(operands);
      continue;
    }

    word_t bin = content->bin;
    word_t data;

    switch (content->type) {
    case DATA_PROCESSING:
      operands = operand_processor(line->operands, 3);
      parse_dp(operands, &bin);
      break;
    case MULTIPLY:
      operands = operand_processor(line->operands, 4);
      parse_mul(&bin, operands, line->opcode);
      break;
    case DATA_TRANSFER:
      /* Calculates the offset and consider the effect of the pipeline */
      operands = operand_processor(line->operands, 3);
      parse_dt(&bin, operands, &data, mcode->length * 4 - line->location_counter - PIPELINE_OFFSET);
      if (data != 0) { /* Append data to the end of the machine code */
        mcode->length++;
        mcode->bin = realloc(mcode->bin, mcode->length * sizeof(word_t));
        mcode->bin[mcode->length - 1] = data;
      }
      break;
    case BRANCH:
      operands = operand_processor(line->operands, 1);
      parse_b(&bin, operands, label_table, line->location_counter);
      break;
    case HALT:
      return mcode;  
    default:
      exceptions(UNKNOWN_INSTRUCTION_TYPE, line->location_counter);
    }

    mcode->bin[i] = bin;
    free_operands(operands);
  }
  return mcode;
}

void free_machine_code(machine_code *mcode) {
  free(mcode->bin);
  free(mcode);
}

// Implementation for the parser helper functions below:
/* DATA_PROCESSING parsing functions*/

static void parse_dp(char **operands, word_t *bin) {
  byte_t opcode_field = (*bin >> OPCODE_LOCATION) & FOUR_BIT_FIELD;
  
  if (compute(opcode_field)) {
    /* result-computing instruction */
    *bin |= to_index(operands[0]) << DP_DT_RD_LOCATION;
    *bin |= to_index(operands[1]) << DP_DT_RN_LOCATION;
    *bin |= parse_operand2(operands[2]); //parse_dp_operand2 will take care of the immediate bit as well
  } else if (move(opcode_field)) {
    /* mov instruction */
    *bin |= to_index(operands[0]) << DP_DT_RD_LOCATION;
    *bin |= parse_operand2(operands[1]); //parse_dp_operand2 will take care of the immediate bit as well
  } else if (flag_set(opcode_field)) {
    /* CPSR flag set instruction */
    *bin |= to_index(operands[0]) << DP_DT_RN_LOCATION;
    *bin |= parse_operand2(operands[1]); //parse_dp_operand2 will take care of the immediate bit as well
  }
}
/*
 * @brief: further tokenizing the operand2 field in DATA_PROCESSING and offset field in DATA_TRANSFER
 * @return: a binary representation of the instruction set according to the given operand2/offset
 */
static word_t parse_operand2(char *operand2) {
  word_t bin = 0;
  if (hash(operand2)) {
    /* processing hash expressions in DATA_PROCESSING*/
    bin |= 1 << IMM_LOCATION;
    long imm = to_index(operand2);
    if (imm >= 0 && imm < 256)  {
      /* if the hash constant can be represented using 8 bits, then directly set it */
      bin |= imm;
    } else {
      /* Check if the hash constant can be represented using right-rotation */
      int rotation = 0;
      word_t i = (word_t) imm; 
      while (i >= 255 && rotation <= 15) {
        int mask = (i >> 30) & 3;
        i <<= 2;
        i |= mask;
        rotation++;
      }

      if (rotation <= 15 && i <= 255) {
        /* Value can be represented by right-rotated 8-bit iediate field */
        bin |= rotation << OPERAND2_ROTATE_LOCATION;
        bin |= i;
      } else {
        /* cannot be represented using the right-rotated 8-bits, throw an error */
        exceptions(IMMEDIATE_VALUE_OUT_OF_BOUND, 0x00000000);
      }
    }
  } else {
    /* processing shifted register in DATA_PROCESSING */
    char *base_reg = strtok(operand2, ",");
    bin |= to_index(base_reg);
    char *shift_name = strtok(NULL, " ");
    if (shift_name != NULL) {
      char *shift_name_list[] = {"lsl", "lsr", "asr", "ror"};
      for (int i = 0; i < 4; i++) {
        if (strcmp(shift_name_list[i], shift_name) == 0) {
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
          /* How should the address field be handled properly? */
          exceptions(IMMEDIATE_VALUE_OUT_OF_BOUND, 0x00000000);
        }
      } else {
        bin |= 1 << OPERAND2_SHIFT_SPEC_LOCATION;
        bin |= shamt << OPERAND2_REGISTER_SHIFT_LOCATION;
      }
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
  *bin |= (to_index(operands[0]) & FOUR_BIT_FIELD) << MUL_RD_LOCATION;
  /* Sets Rm register */
  *bin |= (to_index(operands[1]) & FOUR_BIT_FIELD) << MUL_RM_LOCATION;
  /* Sets Rs register */
  *bin |= (to_index(operands[2]) & FOUR_BIT_FIELD) << MUL_RS_LOCATION;
  /* Sets Rn register */
  if (strcmp(mnemonic, "mla") == 0) {
    *bin |= (to_index(operands[3]) & FOUR_BIT_FIELD) << MUL_RN_LOCATION;
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
  *bin |= (to_index(operands[0]) & FOUR_BIT_FIELD) << DP_DT_RD_LOCATION;
  /* Pre-indexing if no comma can be found in the second operand OR there are only two operands */
  bool pre_index = operands[2] == NULL || (open_brak(operands[1]) && close_brak(operands[2]));
  bool up = true;

  if (equal(operands[1])) {  /* Load value (equal expression) */
    *data = to_index(operands[1]);
    /* Sets Rn to PC */
    *bin |= PC << DP_DT_RN_LOCATION;
    *bin |= offset & TWELVE_BIT_FIELD;
  } else if (pre_index) { /* Pre-indexing */
    *bin |= (to_index(operands[1] + 1) & FOUR_BIT_FIELD) << DP_DT_RN_LOCATION;
    if (operands[2] != NULL) {
      strtok(operands[2], "]");
    }
  } else { /* Post-indexing */
    *bin |= (to_index(remove_bracket(operands[1])) & FOUR_BIT_FIELD) << DP_DT_RN_LOCATION;
  }

  if (operands[2] != NULL) { /* Parses operand2 */
    up = !(operands[2][0] == '-') && !(operands[2][1] == '-');
    if (hash(operands[2])) { /* Hash expression */
      *bin |= to_index(operands[2] + !up) & TWELVE_BIT_FIELD;
    } else { /* Operand2 */
      *bin |= 1 << IMM_LOCATION;
      *bin |= parse_operand2(operands[2] + !up);
    }
  }

  *bin |= pre_index << P_INDEX_LOCATION;
  *bin |= up << UP_BIT_LOCATION;
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

  *bin |= ((addr - current - PIPELINE_OFFSET) >> 2) & TWENTY_FOUR_BIT_FIELD;
}

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
    strcpy(str, operand);
    char *literal = strtok(str, ",");
    while (literal != NULL && i < field_count) {
        tokens[i] = malloc(strlen(literal) * sizeof(char));
        remove_space(literal);
        strcpy(tokens[i], literal);
        
        if (++i + 1 == field_count) {
          literal = strtok(NULL, "\0");
        } else {
          literal = strtok(NULL, ",");
        }
    }

    tokens = realloc(tokens, (i + 1) * sizeof(char *));
    tokens[i] = NULL;

    return tokens;
}

/*
 * @brief: free an array of string tokens
 * @param: char **tokens: the array of string tokens that need to be freed
 */
static void free_operands(char **tokens) {
  int i = 0;
  while (tokens[i] != NULL) {
    free(tokens[i]);
    i++;
  }

  free(tokens);
}
