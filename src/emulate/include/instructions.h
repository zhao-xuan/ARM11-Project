/*
 * Describes the data structure for decoded instructions.
 * This includes the structures for 4 different types of instructions respectively
 * and a wrapper structure.
 */

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdbool.h>
#include "global.h"

/* Two subtypes of operand2 field of DATA PROCESSING and DATA TRANSFER */
typedef struct
{
    byte_t imm;
    byte_t rotate;
} imm_value_t;

typedef struct
{
    byte_t shift_type;
    byte_t rm;
    bool shift_spec;
    union {
        byte_t integer_shift;
        byte_t shift_reg;
    } shift;
} register_form_t;

/* DATA PROCESSING */
typedef struct
{
    byte_t rn;
    byte_t rd;
    byte_t opcode;
    bool imm_const;
    bool set;
    union {
        imm_value_t *imm_value;
        register_form_t *reg_value;
    } operand2;
} data_processing_t;

/* MULTIPLY */
typedef struct
{
    byte_t rm;
    byte_t rd;
    byte_t rs;
    byte_t rn;
    bool accumulate;
    bool set;
} multiply_t;

/* DATA TRANSFER */
typedef struct
{
    byte_t rn;
    byte_t rd;
    bool imm_offset;
    bool pre_index;
    bool up_bit;
    bool load;
    union {
        word_t imm_value;
        register_form_t *reg_value;
    } offset;
} data_transfer_t;

/* BRANCH */
typedef struct
{
    word_t offset;
} branch_t;

/*
 * Enum of instruction types
 */
enum InstructionType
{
    DATA_PROCESSING,
    MULTIPLY,
    BRANCH,
    DATA_TRANSFER,
    HALT,
    EMPTY
};

/*
 * A wrapper for all instructions.
 * The execute function can infer the type of the instruction from "type" enum field..
 */

typedef struct
{
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
