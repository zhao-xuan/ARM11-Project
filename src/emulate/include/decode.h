/* 
 * Decode the instruction and modify the pipeline status accordingly. 
 */

#ifndef DECODE_H
#define DECODE_H

#include "state.h"

/*
 * @brief: Takes a 32-bit binary instruction and decode it into instruction_t representation 
 * @param binary: a 32-bit unsigned integer representing the binary instruction
 * @returns *instruction: pointer to the decoded instruction
 */
instruction_t *decode(const word_t binary);

/*
 * Determines the instruction type
 * @param binary: a 32-bit unsigned integer representing the binary instruction
 * @returns type: enum for instruction type
 */
enum InstructionType check_instruction_type(const word_t binary);

#endif
