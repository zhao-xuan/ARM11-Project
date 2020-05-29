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


/* Opcodes */

/* Unknown Opcode for ALU */
#define UNKNOWN_OPCODE 404
/* Bitwise And */
#define AND_OPCODE 0
/* Bitwise Exclusive Or */
#define EOR_OPCODE 1
/* Arithmetic Subtraction */
#define SUB_OPCODE 2
/* Reversed Arithmetic Subtraction */
#define RSB_OPCODE 3
/* Arithmetic Addition */
#define ADD_OPCODE 4
/* Test */
#define TST_OPCODE 8
/* Test Equal */
#define TEQ_OPCODE 9
/* Compare */
#define CMP_OPCODE 10
/* Bitwise Or */
#define ORR_OPCODE 12
/* Mov */
#define MOV_OPCODE 13



/* Type Aliases */

/* Define bytes */
typedef uint8_t byte_t;
/* Define words */
typedef uint32_t word_t;
/* Define memory address */
typedef uint16_t address_t;
/* Define flag position */
typedef int flag_t;

#endif

