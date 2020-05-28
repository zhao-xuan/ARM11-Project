/* 
	Decode the instruction and modify the pipeline status accordingly. 
 */

#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>
#include "state.h"
#include "instructions.h"
#include "global.h"

/* Spec from Hoang
 * @brief: Take a 32-bit instruction and decode it into instruction_t representation that can be executed in execute.c
 * @param: word_t binary: a 32-bit unsigned integer representing the instruction
 * @return: instruction_t *instr: pointer to the struct, contains an enum field represents instruction type and the actual instruction struct
 */

instruction_t *decode(const word_t binary);

/* @brief: shift and compare 26th, 27th and 4-7th bits to determine the instruction type
 * @param: uint32_t binary: a 32-bit unsigned integer representing the instruction
 * @return: Instructions_type type: enum for instruction type
 */

enum InstructionType check_instruction_type(const word_t binary);




#endif
