#include "execute.h"

static int data_processing_execute(data_processing_t *dp_instr);
static int multiply_execute(multiply_t *mul_instr);
static int data_transfer_execute(data_transfer_t *dt_instr);
static int branch_execute(branch_t *b_instr);

int execute(instruction_t instr_to_exec) {
  if (cond_check(instr_to_exec.cond)) {
    switch (instr_to_exec.type) {
      case DATA_PROCESSING:
        return data_processing_execute(
            instr_to_exec.instructions.data_processing);
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
  } else {
    return 0;
  }
}

static bool cond_check(byte_t cond) {
  switch (cond) {
    /* eq */
    case 0b0000:
      return get_flag(Z_FLAG);
    /* ge */
    case 0b1010:
      return get_flag(N_FLAG) == get_flag(V_FLAG);
    /* gt */
    case 0b1100:
      return !get_flag(Z_FLAG) && cond_check(0b1010);
    /* al */
    case 0b1110:
      return true;
    default:
      return !cond_check(cond - 1);
  }
}

/* Below are helper functions of executing different types of instructions.*/

/*
 * @param: data_processing_t dp_instr: the representation of a data processing
 * instruction
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
  word_t result = mul_instr->rm * mul_instr->rs;
  if(mul_instr->accumulate){
    result += mul_instr->rn;
  }
  if(mul_instr->set){
    set_flag_to(N_FLAG, (result >> 31) & 1U);
    set_flag_to(Z_FLAG, (result >> 31) & 1U);
  }
  set_reg(mul_instr->rd, result);
  return 0;
}

/*
 * @param: data_transfer dt_instr: the representation of a data transfer
 * instruction
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
  word_t curr = get_reg(PC);
  set_reg(PC, b_instr->offset << 2 + curr);
  return 0;
}
