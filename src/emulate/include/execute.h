/* 
 * Executes the instruction and modify machine state accordingly.  
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include "logicunit.h"

/*
 * @brief: executes a single instruction
 * @param instr: pointer to a decoded instruction
 * @returns 0 if succeeded, -1 if error has occurred
 */

int execute(instruction_t *instr);

#endif
