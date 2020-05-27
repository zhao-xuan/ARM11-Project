/*
 * Defines the state of the machine
 */

#ifndef STATE_H
#define STATE_H

#include "global.h"
#include "instructions.h"

/* Structure for Memory, Registers and the Pipeline */
typedef struct{
  /* Memory */
  byte_t memory[MEM_ADDR];
  /* Registers */
  word_t registers[REG_NUM];
  /* Instruction that has already been fetched in the pipeline */
  word_t fetched_instruction;
  /* Instruction that has already been decoded in the pipeline */
  instruction_t decoded_instruction;
} state_t;

#endif
