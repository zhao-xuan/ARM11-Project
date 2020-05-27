#include "include/decode.h"

static void data_processing_helper(word_t binary, instruction_t *struct_p, data_processing_t *instr_p);
static void multiply_helper(word_t binary, instruction_t *struct_p, multiply_t *instr_p);
static void data_transfer_helper(word_t binary, instruction_t *struct_p, data_transfer_t *instr_p);
static void branch_helper(word_t binary, instruction_t *struct_p, branch_t *instr_p);

instruction_t *decode(const word_t binary) {
    /*
     * IMPORTANT: this code works with Big-Endian Encoding!
     */

    enum InstructionType instr_type = check_instruction_type(binary);
    instruction_t *instr_struct = malloc(sizeof(instruction_t));

    instr_struct -> type = instr_type;

    switch (instr_type) {
        case DATA_PROCESSING: {
            data_processing_t *instr_ptr = malloc(sizeof(data_processing_t));
            data_processing_helper(binary, instr_struct, instr_ptr);
            break;
        }
        case MULTIPLY: {
            multiply_t *instr_ptr = malloc(sizeof(multiply_t));
            multiply_helper(binary, instr_struct, instr_ptr);
            break;
        }
        case DATA_TRANSFER: {
            data_transfer_t *instr_ptr = malloc(sizeof(data_transfer_t));
            data_transfer_helper(binary, instr_struct, instr_ptr);
            break;
        }
        case BRANCH:{
            branch_t *instr_ptr = malloc(sizeof(branch_t));
            branch_helper(binary, instr_struct, instr_ptr);
            break;
        }
        case HALT:
            fprintf(stderr, "Halted! Do something!");
            break;
        default:
            fprintf(stderr, "Instruction type doesn't match!");
            break;
    }

    return instr_struct;
}

enum InstructionType check_instruction_type(const word_t binary) {
    /* check if it is the all-0 halt instruction */
    if ((binary | 0) == 0) {
        return HALT;
    }

    /* the 26th(index starting with 0) bit of the unsigned integer binary */
    bool digit_26 = binary & (1 << 26);
    /* 27th(index starting with 0) bit of the unsigned integer binary */
    bool digit_27 = binary & (1 << 27);
    /* mul_flag is used to check instruction type Multiply.
     * If the 25th bit is 0 and both 4th and 7th are 1, it is guaranteed that the instruction type is Multiply */
    bool mul_flag = !(binary & (1 << 25)) && (binary & (1 << 4)) && (binary & (1 << 7));

    if (digit_26) {
        return DATA_TRANSFER;
    } else if (digit_27) {
        return BRANCH;
    } else if (mul_flag) {
        return MULTIPLY;
    } else if (!(digit_26 || digit_27)) {
        return DATA_PROCESSING;
    }

    /*Clearly something better can be done here! Maybe another type ERROR in enum?*/
    return HALT;
}

static void data_processing_helper(word_t binary, instruction_t *struct_p, data_processing_t *instr_p) {
    instr_p -> cond = binary & (0xf << 28);
    instr_p -> rn = binary & (0xf << 16);
    instr_p -> rd = binary & (0xf << 12);
    instr_p -> opcode = binary & (0xf << 21);
    instr_p -> operand2 = binary & 0xfff;
    instr_p -> imm_const = binary & (1 << 25);
    instr_p -> set = binary & (1 << 20);
    (struct_p -> instructions).data_processing = *instr_p;
}

static void multiply_helper(word_t binary, instruction_t *struct_p, multiply_t *instr_p) {
    instr_p -> cond = binary & (0xf << 28);
    instr_p -> rm = binary & 0xf;
    instr_p -> rd = binary & (0xf << 16);
    instr_p -> rs = binary & (0xf << 8);
    instr_p -> rn = binary & (0xf << 12);
    instr_p -> accumulate = binary & (1 << 21);
    instr_p -> set = binary & (1 << 20);
    (struct_p -> instructions).multiply = *instr_p;
}

static void data_transfer_helper(word_t binary, instruction_t *struct_p, data_transfer_t *instr_p) {
    instr_p -> offset = binary & 0xfff;
    instr_p -> cond = binary & (0xf << 28);
    instr_p -> rd = binary & (0xf << 12);
    instr_p -> rn = binary & (0xf << 16);
    instr_p -> imm_offset = binary & (1 << 25);
    instr_p -> pre_index = binary & (1 << 24);
    instr_p -> up_bit = binary & (1 << 23);
    instr_p -> load = binary & (1 << 20);
    (struct_p -> instructions).data_transfer = *instr_p;
}

static void branch_helper(word_t binary, instruction_t *struct_p, branch_t *instr_p) {
    instr_p -> cond = binary & (0xf << 28);
    instr_p -> offset = binary & 0xffffff;
    (struct_p -> instructions).branch = *instr_p;
}