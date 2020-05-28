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

word_t fetch_instruction(state_t state);

/*
 * @brief: put the decoded instruction in state_t
 * @param: state_t state: memory and registers
 * @return: the previous decoded instruction stored in state_t
 */

instruction_t decode_instruction(state_t state);

/*
 * @brief: execute the decoded instruction
 * @param: state_t state: memory and registers
 * @return: 0 if instruction is executed successfully, -1 if error occurs
 */

int execute_instruction(state_t state);

/*
 * @brief: simulate a pipeline of ARM processor
 *         pipiline() will intergrate the three functions above
 * @param: state_t state: memory and registers
 * @return: 0 if instruction is executed successfully, -1 if error occurs
 */

int pipeline(state_t state);

#endif
