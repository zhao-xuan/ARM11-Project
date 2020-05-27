/*
 *  Structure for instructions.
 *
 *  The following file declares the interface for:
 *      1. The 4 types of instructions
 *      2. A wrapper instruction which has one of the instruction along with an
 *         enum to identify which instruction it is
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "global.h"


/*
 *  4 types of instructions with its components broken down as in the spec
 *
 *  The order of the parts are different from the spec to minimise padding
 */


/* DATA PROCESSING */
typedef struct {
    byte_t cond;
    byte_t rn;
    byte_t rd;
    byte_t opcode;
    word_t operand2;
    bool imm_const;
    bool set;
} data_processing_t;


/* MULTIPLY */
typedef struct {
    byte_t cond;
    byte_t rm;
    byte_t rd;
    byte_t rs;
    byte_t rn;
    bool accmulate;
    bool set;
} multiply_t;



/* DATA TRANSFER */
typedef struct {
    address_t offset;
    byte_t cond;
    byte_t rn;
    byte_t rd;
    bool imm_offset;
    bool pre_index;
    bool up_bit;
    bool load;
} data_transfer_t;



/* BRANCH */
typedef struct {
    address_t offset;
    byte_t cond;
} branch_t;



/*
 *   Enum InstructionsType
 *
 */
enum InstructionType {DATA_PROCESSINNG, MULTIPLY, BRANCH, DATA_TRANSFER, HALT};


/*
 *   A generic instruction_t wrapper for all instructions
 *   The executor can then infer what instruction it is from the given Enum
 */

typedef struct {
    enum InstructionType type;
    union {
        data_processing_t data_processing;
        multiply_t multiply;
        data_transfer_t data_transfer;
        branch_t branch;
    } instructions;
} instruction_t;



#endif
