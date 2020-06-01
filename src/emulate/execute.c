#include "execute.h"
#include "logicunit.h"

static int data_processing_execute(data_processing_t *dp_instr);
static int multiply_execute(multiply_t *mul_instr);
static int data_transfer_execute(data_transfer_t *dt_instr);
static int branch_execute(branch_t *b_instr);
static bool write_result(byte_t opcode);

int execute(instruction_t* instr_to_exec) {
    switch (instr_to_exec->type) {
        case DATA_PROCESSING:
            return data_processing_execute(instr_to_exec->instructions.data_processing);
        case MULTIPLY:
            return multiply_execute(instr_to_exec->instructions.multiply);
        case DATA_TRANSFER:
            return data_transfer_execute(instr_to_exec->instructions.data_transfer);
        case BRANCH:
            return branch_execute(instr_to_exec->instructions.branch);
        default:
            fprintf(stderr, "Instruction Type Error!");
            exit(EXIT_FAILURE);
    }
}


/*
 * Data processing instructions
 * @param: data_processing_t dp_instr: the representation of a data processing instruction
 * @return: 0 if succeeded, -1 if error has occurred
 */
static int data_processing_execute(data_processing_t *dp_instr) {
  
  word_t op2, result;

  /* Rotate Right (Operand2 as Immediate Value) */
  if (dp_instr->imm_const) {
    imm_value_t imm = dp_instr->operand2.imm_value;
    shifter(imm.rotate * 2, imm.rotate, &op2, ROR_OPCODE, dp_instr->set);
  } else { /* Operand2 as a Register */
    byte_t shamt;
    register_form_t reg = dp_instr->operand2.reg_value;
    /* Shift specified by a register */
    if (reg.shift_spec) {
      /* Cast word_t to byte_t, assuming shift amount doesn't exceed MAX_BYTE */
      shamt = get_reg(reg.shift.shift_reg);
    } else { /* Use immediate value as shift amount */
      shamt = reg.shift.integer_shift;
    }
    shifter(shamt, get_reg(reg.rm), &op2, reg.shift_type, dp_instr->set);
  }

  alu(get_reg(dp_instr->rn), op2, &result, dp_instr->opcode, dp_instr->set);
  /* Check if the result is written */
  if (write_result(dp_instr->opcode)) {
    set_reg(dp_instr, result);
  }
  
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

static bool write_result(byte_t opcode) {
  if ((opcode >= 0b1000) && (opcode <= 0b1010)) {
    return false;
  }
  return true;
}
