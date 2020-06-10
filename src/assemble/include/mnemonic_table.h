

#ifndef MNEMONIC_TABLE_H
#define MNEMONIC_TABLE_H

#include "global.h"

typedef struct {
  word_t bin;
  enum InstructionType type;
} mnemonic_t, *mnemonic_p;

/* Initialises mnemonic table
 * @returns:
 *    - False iff anything fails (most likely being
 *      memory allocation failure
 */
bool init_mnemonic_table();


/*
 *  @returns: a pointer to mnemonic_t which includes:
 *    - type: enum of instruction type
 *    - bin: partially set binary representation of instruction
 *           based on the rules below, bits which are not set
 *           have the value 0
 *  
 *  1. Data processing
 *    a. [20-24], [26-31] are set
 *    b. Operand2, Rn, Rd and I-bit are not set
 *
 *  2.Multiply
 *    a. [4-7], [20-31] are set
 *    b. Rn, Rm, Rd and Rs are not
 *
 *  3. Single Data Transfer
 *    a. [20-22], [26-31] are set
 *    b. Offset, Rn, Rd, U, P and I-bit are not
 *
 *  4. Branch
 *    a. [24-31] are set
 *    b. Offset is not
 *  
 */
mnemonic_p get_mnemonic_data(char *mnemonic);

/* Free allocated resources to table */
void free_mnemonic_table();

#endif 
