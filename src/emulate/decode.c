#include "decode.h"

static void data_processing_helper(word_t binary, instruction_t *struct_p, data_processing_t *instr_p);
static void multiply_helper(word_t binary, instruction_t *struct_p, multiply_t *instr_p);
static void data_transfer_helper(word_t binary, instruction_t *struct_p, data_transfer_t *instr_p);
static void branch_helper(word_t binary, instruction_t *struct_p, branch_t *instr_p);
/* The two functions below are for debugging purpose*/
static void print_decoded(word_t binary, instruction_t *instr);
static void print_bits(uint32_t x, size_t size);

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

    print_decoded(binary, instr_struct);

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
    instr_p -> cond = (binary >> 28) & 0xf;
    instr_p -> rn = (binary >> 16) & 0xf;
    instr_p -> rd = (binary >> 12) & 0xf;
    instr_p -> opcode = (binary >> 21) & 0xf;
    instr_p -> operand2 = binary & 0xfff;
    instr_p -> imm_const = (binary >> 25) & 1;
    instr_p -> set = (binary >> 20) & 1;
    (struct_p -> instructions).data_processing = *instr_p;
}

static void multiply_helper(word_t binary, instruction_t *struct_p, multiply_t *instr_p) {
    instr_p -> cond = (binary >> 28) & 0xf;
    instr_p -> rm = binary & 0xf;
    instr_p -> rd = (binary >> 16) & 0xf;
    instr_p -> rs = (binary >> 8) & 0xf;
    instr_p -> rn = (binary >> 12) & 0xf;
    instr_p -> accumulate = (binary >> 21) & 1;
    instr_p -> set = (binary >> 20) & 1;
    (struct_p -> instructions).multiply = *instr_p;
}

static void data_transfer_helper(word_t binary, instruction_t *struct_p, data_transfer_t *instr_p) {
    instr_p -> offset = binary & 0xfff;
    instr_p -> cond = (binary >> 28) & 0xf;
    instr_p -> rd = (binary >> 12) & 0xf;
    instr_p -> rn = (binary >> 16) & 0xf;
    instr_p -> imm_offset = (binary >> 25) & 1;
    instr_p -> pre_index = (binary >> 24) & 1;
    instr_p -> up_bit = (binary >> 23) & 1;
    instr_p -> load = (binary >> 20) & 1;
    (struct_p -> instructions).data_transfer = *instr_p;
}

static void branch_helper(word_t binary, instruction_t *struct_p, branch_t *instr_p) {
    instr_p -> cond = (binary >> 28) & 0xf;
    instr_p -> offset = binary & 0xffffff;
    (struct_p -> instructions).branch = *instr_p;
}

static void print_decoded(word_t binary, instruction_t *instr) {
    /* This function is written for debugging */
    print_bits(binary, 32);
    if (instr -> type == DATA_PROCESSING) {
        printf("data processing\n");
        printf("cond: "); print_bits(instr -> instructions.data_processing.cond, 4);
        printf("rn: %d\n", instr -> instructions.data_processing.rn);
        printf("rd: %d\n", instr -> instructions.data_processing.rd);
        printf("opcode: "); print_bits(instr -> instructions.data_processing.opcode, 4);
        printf("operand2: %d\n", instr -> instructions.data_processing.operand2);
        printf("immconst: %d\n", instr -> instructions.data_processing.imm_const);
        printf("set: %d\n\n", instr -> instructions.data_processing.set);
    } else if (instr -> type == MULTIPLY) {
        printf("multiply\n");
        printf("cond: "); print_bits(instr -> instructions.multiply.cond, 4);
        printf("rm: %d\n", instr -> instructions.multiply.rm);
        printf("rd: %d\n", instr -> instructions.multiply.rd);
        printf("rs: %d\n", instr -> instructions.multiply.rs);
        printf("rn: %d\n", instr -> instructions.multiply.rn);
        printf("accumulate: %d\n", instr -> instructions.multiply.accumulate);
        printf("set: %d\n\n", instr -> instructions.multiply.set);
    } else if (instr -> type == DATA_TRANSFER) {
        printf("data_transfer\n");
        printf("offset: %d", instr -> instructions.data_transfer.offset);
        printf("cond: "); print_bits(instr -> instructions.data_transfer.cond, 4);
        printf("rn: %d\n", instr -> instructions.data_transfer.rn);
        printf("rd: %d\n", instr -> instructions.data_transfer.rd);
        printf("imm_offset: %d\n", instr -> instructions.data_transfer.imm_offset);
        printf("pre_index: %d\n", instr -> instructions.data_transfer.pre_index);
        printf("up_bit: %d\n", instr -> instructions.data_transfer.up_bit);
        printf("load: %d\n\n", instr -> instructions.data_transfer.load);
    } else if (instr -> type == BRANCH) {
        printf("branch\n");
        printf("offset: %d\n", instr -> instructions.branch.offset);
        printf("cond: "); print_bits(instr -> instructions.branch.cond, 4);
    }
}

static void print_bits(uint32_t x, size_t size) {
    /* This function is written for debugging */
    int i;
    uint32_t mask = 1 << (size - 1);
    for (i = 0; i < size; ++i) {
        if ((x & mask) == 0) {
            printf("0");
        } else {
            printf("1");
        }
        if ((i + 1) % 4 == 0) {
            printf(" ");
        }
        x = x << 1;
    }
    printf("\n");
}