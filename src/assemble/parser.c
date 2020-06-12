#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "exceptions.h"

/* String processing macros */
#define to_index(literal) ((int) strtol(literal + 1, NULL, 0))
#define equal(literal) (literal[0] == '=')
#define hash(literal) (literal[0] == '#')

#define remove_bracket(literal) (strtok(literal + 1, "]"))
#define remove_space(string) for(; isspace(*string); string++)
#define open_brak(literal) (literal[0] == '[')
#define close_brak(literal) (literal[strlen(literal) - 1] == ']')

/* result-computing data processing instructions */
#define is_compute(opcode) (opcode_field <= 4 || opcode_field == 12)
#define is_move(opcode) (opcode_field == 13)
#define is_flag_set(opcode) (opcode_field >= 8 && opcode_field <= 10)


static void parse_dp(char **operands, word_t *bin);
static void parse_mul(word_t *bin, char **operands, const char *mnemonic);
static void parse_dt(word_t *bin, char **operands, word_t *data, address_t offset);
static void parse_ldr_mov(char **operands, assembly_line *line, mnemonic_p *content);
static void parse_b(word_t *bin, char **operands, symbol_table_t *label_table, address_t current);
static void parse_lsl(char **operands, assembly_line *line, mnemonic_p *content, word_t *mcode_bin);

static word_t parse_operand2(char *operand2);
static void free_operands(char **tokens);
static char **operand_processor(const char *operand, int field_count);

/* Free resources allocated to machine code */
void free_machine_code(machine_code *mcode) {
  free(mcode->bin);
  free(mcode);
}

machine_code *parse(assembly_program *program, symbol_table_t *label_table) {
  machine_code *mcode = eMalloc(sizeof(machine_code));
  mcode->length = program->total_lines;
  mcode->bin = eCalloc(mcode->length, sizeof(word_t));

  for (int i = 0; i < program->total_lines; i++) {
    assembly_line *line = program->lines[i];
    char **operands = operand_processor(line->operands, 3);
    mnemonic_p content = get_mnemonic_data(line->opcode);

    /* Special case for ldr interpreted as mov */
    parse_ldr_mov(operands, line, &content);
    free_operands(operands);

    /* Special case for lsl */
    if (strcmp(line->opcode, "lsl") == 0) {
      parse_lsl(operands, line, &content, mcode->bin + i);
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
        mcode->bin = eRealloc(mcode->bin, mcode->length * sizeof(word_t));
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

/* Special instruction helper functions */
static void parse_ldr_mov(char **operands, assembly_line *line, mnemonic_p *content) {
    if (strcmp(line->opcode, "ldr") == 0 
        && operands[1] != NULL 
        && equal(operands[1]) 
        && (to_index(operands[1]) <= 0xFF)) {
      /* Convert ldr to mov instruction */
      *content = get_mnemonic_data("mov");
      *strchr(line->operands, '=') = '#';
      strcpy(line->opcode, "mov");
    }

}

static void parse_lsl(char **operands, assembly_line *line, 
                      mnemonic_p *content, word_t *mcode_bin) {

  *content = get_mnemonic_data("mov");
  char *rn = strtok(line->operands, ",");
  
  char expr[strlen(rn) * 2 + 6];
  strcpy(expr, rn);
  strcat(expr, ",lsl ");     
  strcat(expr, strtok(NULL, ""));      

  operands = eCalloc(3, sizeof(char *));
  operands[0] = rn;
  operands[1] = expr;

  word_t bin = (*content)->bin;
  parse_dp(operands, &bin);
  *mcode_bin = bin;
  free(operands);
} 

/*  Implementation for the parser helper functions below: */

/*  DATA_PROCESSING parsing functions
 *  @param:
 *      - operands: pointer to start of operand string array
 *      - bin: pointer to where 'binary' rep. of instruction is stored
 */
static void parse_dp(char **operands, word_t *bin) {
  byte_t opcode_field = (*bin >> OPCODE_LOCATION) & FOUR_BIT_FIELD;
  
  if (is_compute(opcode_field)) {
    *bin |= to_index(operands[0]) << DP_DT_RD_LOCATION;
    *bin |= to_index(operands[1]) << DP_DT_RN_LOCATION;
    *bin |= parse_operand2(operands[2]); 
  } else if (is_move(opcode_field)) {
    *bin |= to_index(operands[0]) << DP_DT_RD_LOCATION;
    *bin |= parse_operand2(operands[1]); 
  } else if (is_flag_set(opcode_field)) {
    *bin |= to_index(operands[0]) << DP_DT_RN_LOCATION;
    *bin |= parse_operand2(operands[1]);
  }
}

/*
 * @brief: further tokenizing the operand2 field in DATA_PROCESSING and offset field in DATA_TRANSFER
 * @param:
 *    - operand2: pointer to start of operand2 string
 * @return: a binary representation of the instruction set according to the given operand2/offset
 */
static word_t parse_hash_operand2(char *operand2);
static word_t parse_reg_operand2(char *operand2);

static word_t parse_operand2(char *operand2) {
  if hash(operand2) 
    return parse_hash_operand2(operand2);
   else 
    return parse_reg_operand2(operand2); 
}

static word_t parse_hash_operand2(char *operand2) {
  /* Sets I-bit */
  word_t bin = 1 << IMM_LOCATION;
  long imm = to_index(operand2);
  
  /* Immediate constant fits in 8 bits */
  if (imm >= 0 && imm < 256) return bin |= imm;

  /* Check if the hash constant can be represented using right-rotation */
  int rotation = 0;
  word_t rotated_value = imm & 0xffffffff; 
  for (; rotated_value >= 255 && rotation <= 15; rotation++){
    int mask = (rotated_value >> 30) & 3;
    rotated_value <<= 2;
    rotated_value |= mask;
  }
  
  /* Throw an exception if cannot represent using right-rotation */
  if (rotation > 15 || rotated_value > 255) 
    exceptions(IMMEDIATE_VALUE_OUT_OF_BOUND, 0x00000000);
  
  bin |= rotation << OPERAND2_ROTATE_LOCATION;
  bin |= rotated_value;
  return bin;
}

/* Operand2 as shifted register */
static word_t parse_reg_operand2(char *operand2){
  word_t bin = 0;
  char *base_reg = strtok(operand2, ",");
  bin |= to_index(base_reg);
  
  char *shift_name_list[] = {"lsl", "lsr", "asr", "ror"};
  char *shift_name = strtok(NULL, " ");
  if (shift_name == NULL) return bin; 

  for (int shift_type = 0; shift_type < 4; shift_type++) {
    if (strcmp(shift_name_list[shift_type], shift_name) == 0) 
      bin |= shift_type << OPERAND2_SHIFT_TYPE_LOCATION;
  }

  char *shamt_str = strtok(NULL, " ");
  long shamt = to_index(shamt_str);
  if hash(shamt_str) {
    if (shamt < 32) bin |= shamt << OPERAND2_INTEGER_SHIFT_LOCATION;
    else exceptions(SHIFT_AMOUNT_OUT_OF_BOUND, 0x0);
    
  } else {
    bin |= 1 << OPERAND2_SHIFT_SPEC_LOCATION;
    bin |= shamt << OPERAND2_REGISTER_SHIFT_LOCATION;
  }

  return bin;
}
/*
 * Parser for multiplication instructions
 * This function sets bits [19-8], [3-0] of *bin
 *  @param:
 *    - bin: a pointer to the 32-bits binary word parsed by this function
 *    - operands: pointer to array of strings that holds the operands
 *    - mnemonic: mnemonic of the multiplication instruction
 */
static void parse_mul(word_t *bin, char **operands, const char *mnemonic) {
  /* Sets Rd, Rm and Rs registers */
  *bin |= (to_index(operands[0]) & FOUR_BIT_FIELD) << MUL_RD_LOCATION;
  *bin |= (to_index(operands[1]) & FOUR_BIT_FIELD) << MUL_RM_LOCATION;
  *bin |= (to_index(operands[2]) & FOUR_BIT_FIELD) << MUL_RS_LOCATION;
  
  /* Sets Rn register */
  if (strcmp(mnemonic, "mla") == 0) {
    *bin |= (to_index(operands[3]) & FOUR_BIT_FIELD) << MUL_RN_LOCATION;
  }
}

/*
 *  Parser for single data transfer instructions
 *  This function sets bits [25-23], [19-0] of *bin
 *  
 *  @param: 
 *    - bin: a pointer to the 32-bits binary word parsed by this function
 *    - operands: an array of strings that holds the operands
 *    - data: a pointer to the data that need to be appended at the end of the machine code.
 *    - offset: the offset of the current address to the end of the memory, taking into
 *              account of the pipeline
 * 
 *  *data will be set to 0 if no data need to be appended. Note if data need to be appended,
 *  it can't be 0, since such instructions will be interpreted as a mov instruction
 */
static void parse_dt(word_t *bin, char **operands, word_t *data, address_t offset) {
  *data = 0;
  /* Sets Rd Register */
  *bin |= (to_index(operands[0]) & FOUR_BIT_FIELD) << DP_DT_RD_LOCATION;
  /* Pre-indexing if no comma can be found in the second operand OR there are only two operands */
  bool pre_index = operands[2] == NULL || (open_brak(operands[1]) && close_brak(operands[2]));
  bool up = true, imm = false;

  if (equal(operands[1])) {  /* Load value (equal expression) */
    *data = to_index(operands[1]);
    /* Sets Rn to PC */
    *bin |= PC << DP_DT_RN_LOCATION;
    *bin |= offset & TWELVE_BIT_FIELD;
  } else if (pre_index) { /* Pre-indexing */
    *bin |= (to_index(operands[1] + 1) & FOUR_BIT_FIELD) << DP_DT_RN_LOCATION;
    if (operands[2] != NULL) strtok(operands[2], "]");
  } else { /* Post-indexing */
    *bin |= (to_index(remove_bracket(operands[1])) & FOUR_BIT_FIELD) << DP_DT_RN_LOCATION;
  }

  if (operands[2] != NULL) { /* Parses operand2 */
    up = !(operands[2][0] == '-') && !(operands[2][1] == '-');
    if (hash(operands[2])) { /* Hash expression */
      *bin |= to_index(operands[2] + !up) & TWELVE_BIT_FIELD;
    } else { /* Operand2 */
      imm = true;
      *bin |= parse_operand2(operands[2] + !up);
    }
  }

  *bin |= pre_index << P_INDEX_LOCATION;
  *bin |= up << UP_BIT_LOCATION;
  *bin |= imm << IMM_LOCATION;
}


/*
 * Parser for branching instructions
 * This function sets bits [24-0] of *bin
 * @param: 
 *    - bin: a pointer to the 32-bits binary word parsed by this function
 *    - operands: an array of strings that holds the operands
 *    - label_table: a pointer to the label table produced in the first pass
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
 * @param: 
 *    - operand: pointer to start of the operand fields in a string
 *    - field_count: the maximum number of tokens(fields). 
 *           This is needed in situations where <operand2> in DATA_PROCESSING 
 *           and <address> in DATA_TRANSFER should not be further tokenized
 * @return: an array of string representing the operand fields
 */

static char **operand_processor(const char *operand, int field_count) {
  char **tokens = eCalloc(field_count, sizeof(char *));
  char str[strlen(operand) + 1];
  strcpy(str, operand);
  char *literal = strtok(str, ",");
  
  int i = 0;
  while (literal != NULL && i < field_count) {
    tokens[i] = eMalloc(strlen(literal) * sizeof(char));
    remove_space(literal);
    strcpy(tokens[i], literal);
    
    if (++i + 1 == field_count) literal = strtok(NULL, "\0");
    else literal = strtok(NULL, ",");
  }

  tokens = eRealloc(tokens, (i + 1) * sizeof(char *));
  tokens[i] = NULL;

  return tokens;
}

/*
 * @brief: free an array of string tokens
 * @param: 
 *    - tokens: the array of string tokens that need to be freed
 */
static void free_operands(char **tokens) {
  for (int i = 0; tokens[i]; i++) free(tokens[i]);
  free(tokens);
}
