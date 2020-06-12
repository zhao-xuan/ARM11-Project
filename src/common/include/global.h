/*
 * Global Declarations
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdint.h>

/* Maximum length of lines according to spec */
#define MAX_LINE_LENGTH 512

/* Maximum number of lines pulled from my ass */
#define MAX_FILE_LINES 50

/*
 * Enum of instruction types
 */
enum InstructionType {
  DATA_PROCESSING,
  MULTIPLY,
  BRANCH,
  DATA_TRANSFER,
  HALT,
  EMPTY
};

/* The state of the machine */

/* empty instruction values for when pipeline is empty */
#define EMPTY_INSTR 0xffffffff
/* Word length for this architecture */
#define WORD_LENGTH 32
/* Pipeline offset */
#define PIPELINE_OFFSET 8
/* Number of addresses for memory */
#define MEM_ADDR 0x10000
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

/* Instruction decoding definitions. All in BIG ENDIAN encoding */

/* DATA_PROCESSING & DATA_TRANSFER */
/* Location of Cond field for ALL types of instructions*/
#define COND_LOCATION 28
/* Location of OpCode for DATA_PROCESSING*/
#define OPCODE_LOCATION 21
/* Location of operand2 immediate field for DATA_PROCESSING with I = 1 */
#define OPERAND2_IMM_LOCATION 0
/* Location of operand2 rotate field for DATA_PROCESSING with I = 1 */
#define OPERAND2_ROTATE_LOCATION 8
/* Location of operand2 Rm field for DATA_PROCESSING with I = 0 */
#define OPERAND2_RM_LOCATION 0
/* Location of operand2 integer shift field for DATA_PROCESSING with I = 0 */
#define OPERAND2_INTEGER_SHIFT_LOCATION 7
/* Location of operand2 shift register field for DATA_PROCESSING with I = 0 */
#define OPERAND2_REGISTER_SHIFT_LOCATION 8
/* Location of operand2 shift type field for DATA_PROCESSING with I = 0 */
#define OPERAND2_SHIFT_TYPE_LOCATION 5
/* Location of operand2 shift specify field for DATA_PROCESSING with I = 0 */
#define OPERAND2_SHIFT_SPEC_LOCATION 4
/* Location of Set Cond field for DATA_PROCESSING and MULTIPLY*/
#define SET_COND_LOCATION 20
/* Location of Immediate Operand/Offset field for DATA_PROCESSING and
 * DATA_TRANSFER*/
#define IMM_LOCATION 25
/* Location of Rn field for DATA_PROCESSING and DATA_TRANSFER */
#define DP_DT_RN_LOCATION 16
/* Location of Rd field for DATA_PROCESSING and DATA_TRANSFER */
#define DP_DT_RD_LOCATION 12
/* Location of Load/Store field for DATA_TRANSFER*/
#define LOAD_STORE_LOCATION 20
/* Location of Pre/Post Indexing field for DATA_TRANSFER*/
#define P_INDEX_LOCATION 24
/* Location of Up-bit field for DATA_TRANSFER*/
#define UP_BIT_LOCATION 23
/* Location of Offset field for DATA_TRANSFER */
#define DT_OFFSET_LOCATION 0
/*  The constant for DATA_TRANSFER */
#define DT_CONST (1 << 26)

/* MULTIPLY & BRANCH */
/* Location of constant 0b1001 */
#define MUL_CONST (9 << 4)
/* Location of Rn field for MULTIPLY */
#define MUL_RN_LOCATION 12
/* Location of Rd field for MULTIPLY */
#define MUL_RD_LOCATION 16
/* Location of Rs field for MULTIPLY */
#define MUL_RS_LOCATION 8
/* Location of Rm field for MULTIPLY */
#define MUL_RM_LOCATION 0
/* Location of Accumulate field for MULTIPLY*/
#define ACCUMULATE_LOCATION 21
/* Location of Offset field for BRANCH */
#define BRANCH_OFFSET_LOCATION 0
/* Branch constant */
#define BRANCH_CONST (5 << 25)

/* Field size matching definition */
/* Field size of 1 bit */
#define ONE_BIT_FIELD 0x1
/* Field size of 2 btis */
#define TWO_BIT_FIELD 0x3
/* Field size of 4 bits */
#define FOUR_BIT_FIELD 0xf
/* Field size of 5 bits */
#define FIVE_BIT_FIELD 0x1f
/* Field size of 8 bits */
#define EIGHT_BIT_FIELD 0xff
/* Field size of 12 bits */
#define TWELVE_BIT_FIELD 0xfff
/* Field size of 24 bits */
#define TWENTY_FOUR_BIT_FIELD 0xffffff

/* OpCodes */

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
/* Logical Shift Left */
#define LSL_OPCODE 0
/* Logical Shift Right */
#define LSR_OPCODE 1
/* Arithmetic Shift Right */
#define ASR_OPCODE 2
/* Rotate Right */
#define ROR_OPCODE 3

/* Condition Codes */

/* Equals */
#define EQUAL 0
/* Not equals */
#define NOT_EQUAL 1
/* Greater than or equals to */
#define GREATER_EQUAL 10
/* Strictly less than */
#define LESS_THAN 11
/* Strictly greater than */
#define GREATER_THAN 12
/* Less than or equals to */
#define LESS_EQUAL 13
/* Always execute */
#define ALWAYS 14

/* Type Aliases */

/* Define bytes */
typedef uint8_t byte_t;
/* Define words */
typedef uint32_t word_t;
/* Define memory address */
typedef uint32_t address_t;
/* Define flag position */
typedef uint8_t flag_t;

/* Error Codes */
#define REGISTER_INDEX_OUT_OF_BOUND 50
#define MEMORY_INDEX_OUT_OF_BOUND 51
#define SHIFT_AMOUNT_OUT_OF_BOUND 52
#define UNKNOWN_ALU_OPCODE 60
#define UNKNOWN_SHIFT_TYPE 61
#define UNKNOWN_INSTRUCTION_TYPE 70
#define RUN_TIME_INSTRUCTION_ERROR 71
#define IMMEDIATE_VALUE_OUT_OF_BOUND 80

#endif
