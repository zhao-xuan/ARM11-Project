/* 
	Execute the instruction and modify machine state accordingly.  
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include "state.h"
#include "instructions.h"
#include "global.h"

/*
 * @brief: fetch a line of instruction to state_t
 * @param: state_t state: memory and registers
 * @return: the previous instruction stored in state_t
 */

void fetch_instruction(state_t state);

/*
 * @brief: put the decoded instruction in state_t
 * @param: state_t state: memory and registers
 * @return: the previous decoded instruction stored in state_t
 */

void decode_instruction(state_t state);

/*
 * @brief: execute the decoded instruction in the pipeline and fetch/decode new ones
 * @param: state_t state: memory and registers
 * @return: 0 if instruction is executed successfully, -1 if error occurs
 */

int execute_instruction(state_t state);

/*
 * @brief: simulate a pipeline of ARM processor
 *         pipeline() will integrate the three functions above
 * @param: state_t state: memory and registers
 * @return: 0 if instruction is executed successfully, -1 if error occurs
 */

int pipeline(state_t state);

/*
 * @brief: execute a single instruction
 * @param instruction_t instr: a single instruction structure
 * @return: 0 if succeeded, -1 if error has occurred
 */

int execute(instruction_t instr);

#endif
