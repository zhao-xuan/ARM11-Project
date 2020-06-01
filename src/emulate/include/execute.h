/* 
	Execute the instruction and modify machine state accordingly.  
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include "state.h"
#include "instructions.h"
#include "utils.h"
#include "global.h"

/*
 * @brief: execute a single instruction
 * @param instruction_t instr: a single instruction structure
 * @return: 0 if succeeded, -1 if error has occurred
 */

int execute(instruction_t instr);

#endif
