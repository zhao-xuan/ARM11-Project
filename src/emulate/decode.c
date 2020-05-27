#include "include/decode.h"

instruction_t *decode(const uint32_t binary) {
    enum Instructions_type instr_type = check_instruction_type(binary);
    instruction_t *instr_struct = malloc(sizeof(instruction_t));

    instr_struct -> type = instr_type;

    switch (instr_type) {
        case DATA_PROCESSING: {
            data_processing_instruction *instr_ptr = malloc(sizeof(data_processing_instruction));
            instr_ptr -> cond = binary & (0xf << 28);
            instr_ptr -> rn = binary & (0xf << 16);
            instr_ptr -> rd = binary & (0xf << 12);
            instr_ptr -> opcode = binary & (0xf << 21);
            instr_ptr -> operand2 = binary & 0xfff;
            instr_ptr -> imm_const = binary & (1 << 25);
            instr_ptr -> set = binary & (1 << 20);
            (instr_struct -> instructions).instr_data_processing = instr_ptr;
            break;
        }
        case MULTIPLY: {
            multiply_instruction *instr_ptr = malloc(sizeof(multiply_instruction));
            instr_ptr -> cond = binary & (0xf << 28);
            instr_ptr -> rm = binary & 0xf;
            instr_ptr -> rd = binary & (0xf << 16);
            instr_ptr -> rs = binary & (0xf << 8);
            instr_ptr -> rn = binary & (0xf << 12);
            instr_ptr -> accumulate = binary & (1 << 21);
            instr_ptr -> set = binary & (1 << 20);
            (instr_struct -> instructions).instr_multiply = instr_ptr;
            break;
        }
        case DATA_TRANSFER: {
            data_transfer_instruction *instr_ptr = malloc(sizeof(data_transfer_instruction));
            instr_ptr -> offset = binary & 0xfff;
            instr_ptr -> cond = binary & (0xf << 28);
            instr_ptr -> rd = binary & (0xf << 12);
            instr_ptr -> rn = binary & (0xf << 16);
            instr_ptr -> imm_offset = binary & (1 << 25);
            instr_ptr -> pre_index = binary & (1 << 24);
            instr_ptr -> up_bit = binary & (1 << 23);
            instr_ptr -> load = binary & (1 << 20);
            (instr_struct -> instructions).instr_data_transfer = instr_ptr;
            break;
        }
        case BRANCH:{
            branch_instruction *instr_ptr = malloc(sizeof(branch_instruction));
            instr_ptr -> cond = binary & (0xf << 28);
            instr_ptr -> offset = binary & 0xffffff;
            (instr_struct -> instructions).instr_branch = instr_ptr;
            break;
        }
        case HALT:
            printf("Halted! Do something!");
            break;
        default:
            printf("Instruction type doesn't match!");
            break;
    }

    return instr_struct;
}

enum Instructions_type check_instruction_type(const uint32_t binary) {
    //check if it is the all-0 halt instruction
    if ((binary | 0) == 0) {
        return HALT;
    }

    //the 26th(index starting with 0) bit of the unsigned integer binary
    bool digit_26 = binary & (1 << 26);
    //27th(index starting with 0) bit of the unsigned integer binary
    bool digit_27 = binary & (1 << 27);
    //mul_flag is used to check instruction type Multiply. If the 25th bit is 0 and both 4th and 7th are 1, it is guaranteed that the instruction type is Multiply
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

    return HALT;
}
