/* 
 * Executes the instruction and modify machine state accordingly.  
 */

#ifndef EXECUTE_H
#define EXECUTE_H

#include "logicunit.h"

/*
 * @brief: executes a single instruction
 * @param instr: pointer to a decoded instruction
 */

void execute(instruction_t *instr);

#endif
