/* 
 * Global Declarations
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


/* Macro definitions for the state of the machine */

/* Word length for this architecture */
#define WORD_LENGTH 32
/* Number of addresses for memory */
#define MEM_ADDR 65536
/* Number of registers */
#define REG_NUM 17
/* Program Counter Register */
#define PC 15
/* CPSR Register */
#define CPSR 16
/* Negative Flag */
#define N_FLAG 31
/* Zero Flag */
#define Z_FLAG 30
/* Carry Flag */
#define C_FLAG 29
/* Overflow Flag */
#define V_FLAG 28



/* Type Aliases */

/* Define bytes */
typedef uint8_t byte_t;
/* Define words */
typedef uint32_t word_t;
/* Define memory address */
typedef uint16_t address_t;


#endif

