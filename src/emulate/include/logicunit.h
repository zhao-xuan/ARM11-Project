/*
 * @file: the following file declare functions to mimic the operation of an ALU
 * and BarrelShifter mentioned in the spec
 */


#ifndef LOGIC_H
#define LOGIC_H

#include <limits.h>

#include "global.h"
#include "state.h"

#define UNKNOWN_OPCODE 404 


/*
 *  The following functions define require ALU operations
 *  required for Data-Processing-Instructions for part 1
 *  
 *  @param: 
 *    op1, op2: the 2 operands 
 *    cout: pointer to write the carry-out bit into
 *    res: pointer to where to write the result into
 *  
 *  @execution:
 *    If you pass in a null pointer, it will only write the carry out 
 *    *res = op1 <operator> op2
 *      
 *    *cout = see the description in the spec for C-bit 
 *    
 *      NOTICE that this Carryout is ONLY responsible for the C-bit
 *      of the instruction currently being executed
 */
void and(word_t op1, word_t op2, bool *cout, word_t *res);
void eor(word_t op1, word_t op2, bool *cout, word_t *res);
void sub(word_t op1, word_t op2, bool *cout, word_t *res);
void rsb(word_t op1, word_t op2, bool *cout, word_t *res);
void add(word_t op1, word_t op2, bool *cout, word_t *res);
void orr(word_t op1, word_t op2, bool *cout, word_t *res);
void mov(word_t op1, word_t op2, bool *cout, word_t *res);


void lsl(byte_t shamt, word_t op2, bool *cout, word_t *res);
void lsr(byte_t shamt, word_t op2, bool *cout, word_t *res);
void asr(byte_t shamt, word_t op2, bool *cout, word_t *res);
void ror(byte_t shamt, word_t op2, bool *cout, word_t *res);


/* 
 *  Make a call to the ALU
 *  
 *  @param: op1, op2, opcode, set_flag
 *      set_flag: whether or not to set flags NZC in the CPSR
 *
 *
 *  @return: exit status
 *      0 - if everything goes well
 *      1 - Unknown opcode as defined at the top
 */
int alu(word_t op1, word_t op2, byte_t opcode, bool set);

/* Similar to the ALU */
int shifter(word_t op1, word_t op2, byte_t shift_type, bool set);

#endif
