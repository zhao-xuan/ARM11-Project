#ifndef PARSE_STATIC_H
#define PARSE_STATIC_H

/* Private function prototype defitions for parser */

typedef struct {
  mnemonic_p *content;
  assembly_line *line;
  machine_code *mcode;
  symbol_table_t *label_table;
  int cur_line;
} parser_t;

typedef void (*parse_func)(char **, word_t *, parser_t *);

/*
 * Four main helper functions for each instruction type
 * @param operands: pointer to start of operand string array
 * @param bin: pointer to where 'binary' rep. of instruction is stored
 */

static void parse_dp(char **operands, word_t *bin, parser_t *parser_state);
static void parse_ml(char **operands, word_t *bin, parser_t *parser_state);
static void parse_br(char **operands, word_t *bin, parser_t *parser_state);
static void parse_dt(char **operands, word_t *bin, parser_t *parser_state);

/* Special instructions where they are interpreted differently */
static void parse_ldr_mov(char **operands, parser_t * parser_state);
static void parse_lsl_mov(char **operands, word_t *mcode_bin,
                          parser_t * parser_state);

/*
 * @brief: further tokenizing operand2 field in DATA_PROCESSING and offset field in DATA_TRANSFER
 * @param operand2: pointer to start of operand2 string
 * @return: a binary representation of the instruction set according to the given operand2/offset
 */
static word_t parse_operand2(char *operand2);
static word_t parse_hash_operand2(char *operand2);
static word_t parse_reg_operand2(char *operand2);

/*
 * Second-pass: tokenize the operand field
 * @param operand: pointer to start of the operand fields in a string
 * @param field_count: the maximum number of tokens(fields). This is needed in situations where <operand2>
 * in DATA_PROCESSING and <address> in DATA_TRANSFER should not be further tokenized
 * @return: an array of string representing the operand fields
 */
static char **operand_processor(const char *operand, int field_count);

static void free_operands(char **tokens);
#endif
