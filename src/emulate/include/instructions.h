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


/* Two subtypes of operand2 field of DATA_PROCESSING */
typedef struct {
    byte_t imm;
    byte_t rotate;
} imm_value_t;

typedef struct {
    byte_t shift_type;
    byte_t rm;
    bool shift_spec;
    union {
        byte_t integer_shift;
        byte_t shift_reg;
    } shift;
} register_form_t;

/* DATA PROCESSING */
typedef struct {
    byte_t rn;
    byte_t rd;
    byte_t opcode;
    bool imm_const;
    bool set;
    union {
        imm_value_t imm_value;
        register_form_t reg_value;
    } operand2;
} data_processing_t;


/* MULTIPLY */
typedef struct {
    byte_t rm;
    byte_t rd;
    byte_t rs;
    byte_t rn;
    bool accumulate;
    bool set;
} multiply_t;


/* DATA TRANSFER */
typedef struct {
    address_t offset;
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
} branch_t;



/*
 *   Enum Instructions_type
 *       - To help with the while loop for the pipeline
 *       
 */
enum InstructionType {DATA_PROCESSING, MULTIPLY, BRANCH, DATA_TRANSFER, HALT};


/*
 *   A generic instruction_t wrapper for all instructions
 *   The executor can then infer what instruction it is from the given Enum
 */

typedef struct {
    enum InstructionType type;
    byte_t cond;
    union {
        data_processing_t *data_processing;
        multiply_t *multiply;
        data_transfer_t *data_transfer;
        branch_t *branch;
    } instructions;
} instruction_t;



#endif
