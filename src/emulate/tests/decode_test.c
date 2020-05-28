#include "decode_test.h"

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
        printf("offset: %d\n", instr -> instructions.data_transfer.offset);
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
        printf("\n");
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
