#include "instructions.h"

typedef struct parsetable
{
	InstructionType type;
	byte_t cond;
	byte_t opcode;
} parse_table_t;



parse_table_t *init_parse_table();

void free_parse_table(parse_table_t *parse_table);

InstructionType get_type(parse_table_t *parse_table, char *mnemonic);

byte_t get_cond(parse_table_t *parse_table, char *mnemonic);

byte_t get_opcode(parse_table_t *parse_table, char *mnemonic);
