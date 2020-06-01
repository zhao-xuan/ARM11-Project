#include "execute.h"

static int data_processing_execute(data_processing_t *dp_instr);
static int multiply_execute(multiply_t *mul_instr);
static int data_transfer_execute(data_transfer_t *dt_instr);
static int branch_execute(branch_t *b_instr);
static byte_t compute_shift_register(register_form_t reg_value);

int execute(instruction_t instr_to_exec) {
    switch (instr_to_exec.type) {
        case DATA_PROCESSING:
            return data_processing_execute(instr_to_exec.instructions.data_processing);
        case MULTIPLY:
            return multiply_execute(instr_to_exec.instructions.multiply);
        case DATA_TRANSFER:
            return data_transfer_execute(instr_to_exec.instructions.data_transfer);
        case BRANCH:
            return branch_execute(instr_to_exec.instructions.branch);
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
static int data_processing_execute(data_processing_t *dp_instr) {
    /* Data processing instructions should be executed here */

    return 0;
}

/*
 * @param: multiply_t mul_instr: the representation of a multiply instruction
 * @return: 0 if succeeded, -1 if error has occurred
 */
static int multiply_execute(multiply_t *mul_instr) {
    /* Multiply instructions should be executed here */

    return 0;
}

/*
 * @param: data_transfer dt_instr: the representation of a data transfer instruction
 * @return: 0 if succeeded, -1 if error has occurred
 */
static int data_transfer_execute(data_transfer_t *dt_instr) {
    /* Data transfer instructions should be executed here */
    /* We are assuming that PC cannot be Rm or Rd, so the code below does not check this */
    byte_t offset;
    byte_t base_reg = dt_instr -> rn;
    /* Check if the base_reg is PC: if it is PC then add 8 */
    base_reg += base_reg == PC ? 8 : 0;

    bool add_or_sub = dt_instr -> up_bit;

    if (dt_instr -> imm_offset) {
        /* offset is a shift register as shown in operand2 in DATA_PROCESSING */
        offset = compute_shift_register(dt_instr -> reg_value);
    } else {
        /* offset is an immediate */
        offset = dt_instr -> offset;
    }

    if (dt_instr -> pre_index) {
        /* Pre-indexing mode without setting base register */
        offset += add_or_sub ? offset : -offset;
    }

    /* Load or store operations, assume that all 32-bit long(a word) */
    if (dt_instr -> load) {
        set_reg(dt_instr -> rd, get_word(offset));
    } else {
        set_word(offset, dt_instr -> rd);
    }

    if (!(dt_instr -> pre_index)) {
        /* In post-indexing, Rm cannot be the same as Rn */
        byte_t rm = (dt_instr -> offset).reg_value -> rm;
        if (rm == dt_instr -> rn) {
            return -1;
        }
        /* Post-indexing mode AND setting base register */
        address_t result = get_reg(base_reg);
        result += add_or_sub ? offset : -offset;
        set_reg(base_reg, result);
    }

    return 0;
}

/*
 * @param: branch_t b_instr: the representation of a branch instruction
 * @return: 0 if succeeded, -1 if error has occurred
 */
static int branch_execute(branch_t *b_instr) {
    /* Branch instructions should be executed here */

    return 0;
}
/*
 * @brief: compute the shift register and return the shift amount
 * @param: register_form_t reg_value: the shift register representation
 * @return the amount to be shifted
 */
static byte_t compute_shift_register(register_form_t reg_value) {
    byte_t shamt;

    /* Shift specified by a register */
    if (reg_value.shift_spec) {
        /* Cast word_t to byte_t, assuming shift amount doesn't exceed MAX_BYTE */
        shamt = get_reg(reg_value.shift.shift_reg);
    } else {
        /* Use immediate value as shift amount */
        shamt = reg_value.shift.integer_shift;
    }
}


