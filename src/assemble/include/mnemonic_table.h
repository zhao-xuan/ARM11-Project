#ifndef MNEMONIC_TABLE_H
#define MNEMONIC_TABLE_H

#include <stdbool.h>
#include "global.h"

typedef struct {
  word_t bin;
  enum InstructionType type;
} mnemonic_t, *mnemonic_p;

/* 
 * Initialises mnemonic table
 * @returns: False iff anything fails (most likely being memory allocation failure)
 */
bool init_mnemonic_table();

/*
 * @returns: a pointer to mnemonic_t which includes the enum of instruction type
 * and a partially set binary representation of instruction based on the rules below.
 * Bits which are not set have the value 0.
 * Data processing: [20-24], [26-31] are set; Operand2, Rn, Rd and I-bit are not set
 * Multiply [4-7], [20-31] are set; Rn, Rm, Rd and Rs are not set
 * Single Data Transfer: [20-22], [26-31] are set; Offset, Rn, Rd, U, P and I-bit are not
 * Branch: [24-31] are set; Offset is not
 */
mnemonic_p get_mnemonic_data(char *mnemonic);

/* Free allocated resources to table */
void free_mnemonic_table();

#endif
