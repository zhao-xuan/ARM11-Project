/*
 * @file: the following file declare functions to mimic the operation of an ALU
 * and BarrelShifter mentioned in the spec
 */


#ifndef LOGIC_H
#define LOGIC_H


#include <limits.h>

#include "global.h"



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
 *  @brief: An array of function pointer to choose which function to perform
 *          based on the given opcode for the ALU.
 *          
 *          Notice that there are only 7 operations for the ALU as tst, teq and cmp
 *          use other functions
 * 
 *  @usage: 
 *    - For Data_Processing Instruction: you can implement it very nicely as shown 
 *          + No need for case statement     
 *
 *    int index = opcode  > 10 ? opcode % 8 + 1 : opcode % 8;
 *    bool cout;
 *    word_t res, op1 = ..., op2 = ...;
 *    (*alu[index]) (op1, op2, &cout, &res);
 *
 */

void (*alu[7]) (word_t op1, word_t op2, bool *cout, word_t *res); 



/*
 *  @brief: The idea is similar to that of the above. 
 *          The only difference is that we can index straight into this array using 
 *          the ShiftType in the instruction
 */

void (*barrel_shifter[4]) (byte_t shamt, word_t op2, bool *cout, word_t *res);


#endif
