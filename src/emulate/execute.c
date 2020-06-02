/*
 * Implementation of the execution of the instructions.
 */

#include <stdio.h>
#include <stdlib.h>
#include "execute.h"

static int data_processing_execute(data_processing_t *dp_instr);
static int multiply_execute(multiply_t *mul_instr);
static int data_transfer_execute(data_transfer_t *dt_instr);
static int branch_execute(branch_t *b_instr);
static word_t compute_shift_register(register_form_t reg_value, bool set);
static void set_or_load(data_transfer_t *dt_instr, word_t address);
static bool cond_check(byte_t cond);
static bool write_result(byte_t opcode);

int execute(instruction_t *instr_to_exec)
{
  if (cond_check(instr_to_exec->cond))
  {
    switch (instr_to_exec->type)
    {
    case DATA_PROCESSING:
      return data_processing_execute(
          instr_to_exec->instructions.data_processing);
    case MULTIPLY:
      return multiply_execute(instr_to_exec->instructions.multiply);
    case DATA_TRANSFER:
      return data_transfer_execute(instr_to_exec->instructions.data_transfer);
    case BRANCH:
      return branch_execute(instr_to_exec->instructions.branch);
    default:
      fprintf(stderr, "Instruction Type Error! ");
      exit(EXIT_FAILURE);
    }
  }
  else
  {
    return 0;
  }
}

/*
 * Check whether the condition code is satisfied.
 * @param cond: the condition code.
 */
static bool cond_check(byte_t cond)
{
  switch (cond)
  {
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

/*
 * Execute data processing instructions.
 * @param *dp_instr: a pointer to the data processing instruction to be executed.
 * @return: 0 if succeeded, otherwise an error code.
 */
static int data_processing_execute(data_processing_t *dp_instr)
{
  word_t op2, result;

  /* Rotate Right (Operand2 as Immediate Value) */
  if (dp_instr->imm_const)
  {
    imm_value_t imm = *dp_instr->operand2.imm_value;
    shifter(imm.rotate * 2, imm.imm, &op2, ROR_OPCODE, dp_instr->set);
  }
  else
  { /* Operand2 as a Register */
    register_form_t reg = *dp_instr->operand2.reg_value;
    op2 = compute_shift_register(reg, dp_instr->set);
  }

  alu(get_reg(dp_instr->rn), op2, &result, dp_instr->opcode, dp_instr->set);
  /* Check if the result is written */
  if (write_result(dp_instr->opcode))
  {
    set_reg(dp_instr->rd, result);
  }

  return 0;
}

/*
 * Execute multiply instructions.
 * @param *mul_instr: a pointer to the multiply instruction to be executed.
 * @return: 0 if succeeded, otherwise an error code.
 */
static int multiply_execute(multiply_t *mul_instr)
{
  /* Multiply instructions should be executed here */
  word_t rm_val = get_reg(mul_instr->rm);
  word_t rs_val = get_reg(mul_instr->rs);
  word_t result = rm_val * rs_val;

  if (mul_instr->accumulate)
  {
    result += get_reg(mul_instr->rn);
  }
  if (mul_instr->set)
  {
    set_flag_to(N_FLAG, (result >> 31) & 1U);
    set_flag_to(Z_FLAG, result == 0);
  }
  set_reg(mul_instr->rd, result);
  return 0;
}

/*
 * Execute data transfer instructions.
 * @param *dt_instr: a pointer to the data transfer instruction to be executed.
 * @return: 0 if succeeded, -1 if error has occurred
 */
static int data_transfer_execute(data_transfer_t *dt_instr)
{
  /* Assume that PC cannot be Rm or Rd */
  byte_t base_reg = dt_instr->rn;
  word_t offset, address;

  bool up_bit = dt_instr->up_bit;

  if (dt_instr->imm_offset)
  {
    /* offset is a shift register as shown in operand2 in DATA_PROCESSING */
    offset = compute_shift_register(*dt_instr->offset.reg_value, false);
  }
  else
  {
    /* offset is an immediate */
    offset = dt_instr->offset.imm_value;
  }

  address = get_reg(base_reg);
  if (dt_instr->pre_index)
  {
    /* Pre-indexing mode without setting base register */
    if (up_bit)
    {
      address += offset;
    }
    else
    {
      address -= offset;
    }
    set_or_load(dt_instr, address);
  }
  else
  {
    set_or_load(dt_instr, address);
    if (up_bit)
    {
      address += offset;
    }
    else
    {
      address -= offset;
    }
    set_reg(base_reg, address);
  }
  return 0;
}

/*
 * Determines whether writes the result to the register from alu.
 * @param opcode: the opcode of the alu operation.
 * @returns: Whether writes the result or not.
 */
static bool write_result(byte_t opcode)
{
  if ((opcode >= 0b1000) && (opcode <= 0b1010))
  {
    return false;
  }
  return true;
}

/*
 * Execute a branching instruction.
 * @param b_instr: a pointer to the branching instruction to be executed.
 * @return: 0 if succeeded, otherwise an error code.
 */
static int branch_execute(branch_t *b_instr)
{
  word_t val = get_reg(PC);
  val += b_instr->offset;
  set_reg(PC, val);
  empty_pipeline();
  return 0;
}

/* 
 * Load or store the data for executing data transfer instructions.
 * @param *dt_instr: a pointer to the data transfer instruction to be executed
 * @param address: address of the memory to store to/load from.
 */
static void set_or_load(data_transfer_t *dt_instr, word_t address)
{
  if (dt_instr->load)
  {
    set_reg(dt_instr->rd, get_word(address));
  }
  else
  {
    set_word(address, get_reg(dt_instr->rd));
  }
}

/*
 * Compute the result from the barrel shifter.
 * @param reg_value: operand2 of the instruction when interpreted as register.
 * @param set: flags are set by the barrel shifter if set is true.
 * @returns the result computed by the barrel shifter.
 */
static word_t compute_shift_register(register_form_t reg_value, bool set)
{
  byte_t shamt;
  word_t result;
  word_t operand = get_reg(reg_value.rm);
  /* Shift specified by a register */
  if (reg_value.shift_spec)
  {
    /* Cast word_t to byte_t, assuming shift amount doesn't exceed MAX_BYTE */
    shamt = get_reg(reg_value.shift.shift_reg);
  }
  else
  {
    /* Use immediate value as shift amount */
    shamt = reg_value.shift.integer_shift;
  }

  shifter(shamt, operand, &result, reg_value.shift_type, set);
  return result;
}
