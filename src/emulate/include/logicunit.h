/*
 * @file: the following file declare functions to mimic the operation of an ALU
 * and BarrelShifter mentioned in the spec
 */


#ifndef LOGIC_H
#define LOGIC_H

#include "state.h"

/* 
 *  Make a call to the ALU and Barrel Shifter
 *  
 *  @param: op1, op2, *result, opcode, set_flag
 *      set: whether or not to set flags NZC in the CPSR
 *      *result: Address to which teh result is stored
 *
 *
 *  @return: exit status
 *            0        - if everything goes well
 *      UNKNOWN_OPCODE - Unknown opcode as defined at the top
 */
int alu(word_t op1, word_t op2, word_t *result, byte_t opcode, bool set);

/* Similar to the ALU */
int shifter(byte_t shamt, word_t operand, word_t *result, byte_t shift_type, bool set);

#endif
