/*
 * Declares functions to mimic the operation of the ALU and barrel shifter in an ARM11 processor
 */

#ifndef LOGIC_H
#define LOGIC_H

#include "state.h"

/* 
 * Perform an ALU operation
 * @param op1: operand 1
 * @param op2: operand 2
 * @param *result: pointer to the result field
 * @param opcode: opcode for the ALU operation
 * @param set: the CPSR register will be modified if set is true
 */
void alu(word_t op1, word_t op2, word_t *result, byte_t opcode, bool set);

/* 
 * Perform an barrel shifter operation
 * @param shamt: shift/rotate amount
 * @param operand: the input value 
 * @param *result: pointer to the result field
 * @param shift_type: opcode for the barrel shifter operation
 * @param set: the CPSR register will be modified if set is true
 */
void shifter(byte_t shamt, word_t operand, word_t *result, byte_t shift_type, bool set);

#endif
