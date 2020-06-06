#include "instructions.h"

typedef struct parsetable
{
	InstructionType type;
	byte_t cond;
} parse_table_t;

parse_table_t *init_parse_table();

void free_parse_table(parse_table_t *parse_table);

InstructionType get_type(char *mnemonic);

byte_t get_cond(char *mnemonic);