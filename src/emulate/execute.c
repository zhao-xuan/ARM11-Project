#include "execute.h"

int execute(instruction_t instr_to_exec) {
    switch (instr_to_exec.type) {
        case DATA_PROCESSING:
            return data_processing_execute(instr_to_exec.instructions.data_processing);
            break;
        case MULTIPLY:
            return multiply_execute(instr_to_exec.instructions.multiply);
            break;
        case DATA_TRANSFER:
            return data_transfer_execute(instr_to_exec.instructions.data_transfer);
            break;
        case BRANCH:
            return branch_execute(instr_to_exec.instructions.branch);
            break;
        default:
            fprintf(stderr, "Instruction Type Error!");
            exit(EXIT_FAILURE);
    }
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