#include "execute.h"

void fetch_instruction(state_t state) {
    state.fetched_instruction = state.memory[get_reg(PC)];
    set_reg(PC, get_reg(PC) + 4);
}

void decode_instruction(state_t state) {
    state.decoded_instruction = *(decode(state.fetched_instruction));
    fetch_instruction(state);
}

int execute_instruction(state_t state) {
    intruction_t instr_to_exec = state.decoded_instruction;
    decode_instruction(state);
    execute();
}

int pipeline(state_t state) {
    while (true) {
        fetch_instruction(state);
        decode_instruction(state);
        execute_instruction(state);
    }
}

int execute(instruction_t instr_to_exec) {
    switch (instr_to_exec.type) {
        case DATA_PROCESSING:
            data_processing_execute(instr_to_exec.instructions.data_processing);
            break;
        case MULTIPLY:
            multiply_execute(instr_to_exec.instructions.multiply);
            break;
        case DATA_TRANSFER:
            data_transfer_execute(instr_to_exec.instructions.data_transfer);
            break;
        case BRANCH:
            branch_execute(instr_to_exec.instructions.branch);
            break;
        default:
            fprintf(stderr, "Instruction Type Error!");
    }

    return 0;
}

/* Below are helper functions of executing different types of instructions.*/

/*
 * @param: data_processing_t dp_instr: the representation of a data processing instruction
 * @return: 0 if succeeded, -1 if error has occurred
 */
static int data_processing_execute(data_processing_t dp_instr) {
    /* Data processing instructions should be executed here */

    return 0;
}

/*
 * @param: multiply_t dp_instr: the representation of a multiply instruction
 * @return: 0 if succeeded, -1 if error has occurred
 */
static int multiply_execute(multiply_t mul_instr) {
    /* Multiply instructions should be executed here */

    return 0;
}

/*
 * @param: data_transfer dp_instr: the representation of a data transfer instruction
 * @return: 0 if succeeded, -1 if error has occurred
 */
static int data_transfer_execute(data_transfer_t dt_instr) {
    /* Data transfer instructions should be executed here */

    return 0;
}

/*
 * @param: branch_t dp_instr: the representation of a branch instruction
 * @return: 0 if succeeded, -1 if error has occurred
 */
static int branch_execute(data_processing_t dp_instr) {
    /* Branch instructions should be executed here */

    return 0;
}