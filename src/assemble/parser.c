#include "parser.h"

#include <ctype.h>
#include <string.h>

#include "exceptions.h"
#include "parser_private_defns.h"

/* String processing macros */
#define to_index(literal) ((int)strtol(literal + 1, NULL, 0))
#define is_equal(literal) (literal[0] == '=')
#define is_hash(literal) (literal[0] == '#')

#define remove_bracket(literal) (strtok(literal + 1, "]"))
#define remove_space(string) for (; isspace(*string); string++)
#define open_brak(literal) (literal[0] == '[')
#define close_brak(literal) (literal[strlen(literal) - 1] == ']')

/* result-computing data processing instructions */
#define is_compute(opcode) (opcode_field <= 4 || opcode_field == 12)
#define is_move(opcode) (opcode_field == 13)
#define is_flag_set(opcode) (opcode_field >= 8 && opcode_field <= 10)

/*  data processing -> multiply -> branch -> data transfer */
static const int parser_func_maxfield[] = {3, 4, 1, 3};
static const parse_func parser_helper[] = {&parse_dp, &parse_ml, &parse_br,
                                           &parse_dt};

void free_machine_code(machine_code *mcode) {
  free(mcode->bin);
  free(mcode);
}

machine_code *parse(assembly_program *program, symbol_table_t *label_table) {
  machine_code *mcode = eMalloc(sizeof(machine_code));
  mcode->length = program->total_lines;
  mcode->bin = eCalloc(mcode->length, sizeof(word_t));

  parser_p parser_pipe = eMalloc(sizeof(parser_t));
  parser_pipe->mcode = mcode;
  parser_pipe->label_table = label_table;

  for (int i = 0; i < program->total_lines; i++) {
    assembly_line *line = program->lines[i];
    char **operands = operand_processor(line->operands, 3);
    mnemonic_p content = get_mnemonic_data(line->opcode);

    parser_pipe->line = line;
    parser_pipe->content = &content;

    /* Special case for ldr interpreted as mov */
    parse_ldr_mov(operands, parser_pipe);
    free_operands(operands);

    /* Special case for lsl */
    if (strcmp(line->opcode, "lsl") == 0) {
      parse_lsl_mov(operands, mcode->bin + i, parser_pipe);
      continue;
    }

    word_t bin = content->bin;

    switch (content->type) {
      case HALT:
        return mcode;
      case EMPTY:
        exceptions(UNKNOWN_INSTRUCTION_TYPE, line->location_counter);
      default:
        operands = operand_processor(line->operands,
                                     parser_func_maxfield[content->type]);
        parser_helper[content->type](operands, &bin, parser_pipe);
    }

    mcode->bin[i] = bin;
    free_operands(operands);
  }
  free(parser_pipe);
  return mcode;
}

/* Special instruction helper functions */
static void parse_ldr_mov(char **operands, parser_p parser_pipe) {
  assembly_line *cur_line = parser_pipe->line;
  if (strcmp(cur_line->opcode, "ldr") == 0 && operands[1] != NULL &&
      is_equal(operands[1]) && (to_index(operands[1]) <= 0xFF)) {
    /* Convert ldr to mov instruction */
    *parser_pipe->content = get_mnemonic_data("mov");
    *strchr(cur_line->operands, '=') = '#';
    strcpy(cur_line->opcode, "mov");
  }
}

static void parse_lsl_mov(char **operands, word_t *mcode_bin,
                          parser_p parser_pipe) {
  *parser_pipe->content = get_mnemonic_data("mov");
  char *rn = strtok(parser_pipe->line->operands, ",");

  char expr[strlen(rn) * 2 + 6];
  strcpy(expr, rn);
  strcat(expr, ",lsl ");
  strcat(expr, strtok(NULL, ""));

  operands = eCalloc(3, sizeof(char *));
  operands[0] = rn;
  operands[1] = expr;

  word_t bin = (*parser_pipe->content)->bin;
  parse_dp(operands, &bin, parser_pipe);
  *mcode_bin = bin;
  free(operands);
}

static void parse_dp(char **operands, word_t *bin, parser_p parser_pipe) {
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

static word_t parse_operand2(char *operand2) {
  return is_hash(operand2) ? parse_hash_operand2(operand2)
                           : parse_reg_operand2(operand2);
}

static word_t parse_hash_operand2(char *operand2) {
  long imm = to_index(operand2);

  /* Sets I-bit */
  word_t bin = 1 << IMM_LOCATION;

  /* Immediate constant fits in 8 bits */
  if (imm >= 0 && imm < 256) return bin |= imm;

  /* Check if the hash constant can be represented using right-rotation */
  int rotation = 0;
  word_t rotated_value = imm & 0xffffffff;
  for (; rotated_value >= 255 && rotation <= 15; rotation++) {
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

static word_t parse_reg_operand2(char *operand2) {
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
  if
    is_hash(shamt_str) {
      if (shamt >= 32) exceptions(SHIFT_AMOUNT_OUT_OF_BOUND, 0x0);
      bin |= shamt << OPERAND2_INTEGER_SHIFT_LOCATION;
    }
  else {
    bin |= 1 << OPERAND2_SHIFT_SPEC_LOCATION;
    bin |= shamt << OPERAND2_REGISTER_SHIFT_LOCATION;
  }

  return bin;
}

static void parse_ml(char **operands, word_t *bin, parser_p parser_pipe) {
  /* Sets Rd, Rm and Rs registers */
  *bin |= (to_index(operands[0]) & FOUR_BIT_FIELD) << MUL_RD_LOCATION;
  *bin |= (to_index(operands[1]) & FOUR_BIT_FIELD) << MUL_RM_LOCATION;
  *bin |= (to_index(operands[2]) & FOUR_BIT_FIELD) << MUL_RS_LOCATION;

  /* Sets Rn register */
  if (strcmp(parser_pipe->line->opcode, "mla") == 0) {
    *bin |= (to_index(operands[3]) & FOUR_BIT_FIELD) << MUL_RN_LOCATION;
  }
}

static void parse_dt(char **operands, word_t *bin, parser_p parser_pipe) {
  word_t data = 0;
  machine_code *mcode = parser_pipe->mcode;
  address_t offset =
      mcode->length * 4 - parser_pipe->line->location_counter - PIPELINE_OFFSET;

  /* Sets Rd Register */
  *bin |= (to_index(operands[0]) & FOUR_BIT_FIELD) << DP_DT_RD_LOCATION;

  /* Pre-indexing if no comma can be found in the second operand OR there are
   * only two operands */
  bool pre_index = operands[2] == NULL ||
                   (open_brak(operands[1]) && close_brak(operands[2]));
  bool up = true, imm = false;

  if (is_equal(operands[1])) { /* Load value (equal expression) */
    data = to_index(operands[1]);
    /* Sets Rn to PC */
    *bin |= PC << DP_DT_RN_LOCATION;
    *bin |= offset & TWELVE_BIT_FIELD;
  } else if (pre_index) { /* Pre-indexing */
    *bin |= (to_index(operands[1] + 1) & FOUR_BIT_FIELD) << DP_DT_RN_LOCATION;
    if (operands[2] != NULL) strtok(operands[2], "]");
  } else { /* Post-indexing */
    *bin |= (to_index(remove_bracket(operands[1])) & FOUR_BIT_FIELD)
            << DP_DT_RN_LOCATION;
  }

  if (operands[2] != NULL) {
    up = !(operands[2][0] == '-') && !(operands[2][1] == '-');
    imm = !is_hash(operands[2]);

    *bin |= is_hash(operands[2])
                ? to_index(operands[2] + !up) & TWELVE_BIT_FIELD /* Hash Expr */
                : parse_operand2(operands[2] + !up);             /* Operand2  */
  }

  *bin |= imm << IMM_LOCATION;
  *bin |= up << UP_BIT_LOCATION;
  *bin |= pre_index << P_INDEX_LOCATION;

  if (data != 0) {
    /* Append data to the end of the machine code */
    mcode->length++;
    mcode->bin = eRealloc(mcode->bin, mcode->length * sizeof(word_t));
    mcode->bin[mcode->length - 1] = data;
  }
}

static void parse_br(char **operands, word_t *bin, parser_p parser_pipe) {
  char *errptr = NULL;
  address_t cur_addr = parser_pipe->line->location_counter;
  word_t addr = (word_t)strtol(operands[0] + 1, &errptr, 0);
  /* Expression is a label, not a number. See documentation for strtol */
  if (errptr != NULL) {
    addr = get_label_address(parser_pipe->label_table, operands[0]);
  }

  *bin |= ((addr - cur_addr - PIPELINE_OFFSET) >> 2) & TWENTY_FOUR_BIT_FIELD;
}

static char **operand_processor(const char *operand, int field_count) {
  char **tokens = eCalloc(field_count, sizeof(char *));
  char str[strlen(operand) + 1];
  strcpy(str, operand);
  char *literal = strtok(str, ",");

  int i = 0;
  while (literal != NULL && i < field_count) {
    tokens[i] = eCalloc(strlen(literal) + 1, sizeof(char));
    remove_space(literal);
    strcpy(tokens[i], literal);

    if (++i + 1 == field_count)
      literal = strtok(NULL, "\0");
    else
      literal = strtok(NULL, ",");
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
