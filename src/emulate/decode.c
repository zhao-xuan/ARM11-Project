/*
 * Implementation of decoding instructions
 */

#include "decode.h"

#include <stdio.h>

#include "exceptions.h"
#include "utils.h"

static void data_processing_helper(word_t binary, instruction_t *struct_p,
                                   data_processing_t *instr_p);
static void multiply_helper(word_t binary, instruction_t *struct_p,
                            multiply_t *instr_p);
static void data_transfer_helper(word_t binary, instruction_t *struct_p,
                                 data_transfer_t *instr_p);
static void branch_helper(word_t binary, instruction_t *struct_p,
                          branch_t *instr_p);

instruction_t *decode(const word_t binary) {
  enum InstructionType instr_type = check_instruction_type(binary);
  instruction_t *instr_struct = eMalloc(sizeof(instruction_t));

  instr_struct->type = instr_type;
  instr_struct->cond = (binary >> COND_LOCATION) & FOUR_BIT_FIELD;

  switch (instr_type) {
    case DATA_PROCESSING: {
      data_processing_t *instr_ptr = eMalloc(sizeof(data_processing_t));
      data_processing_helper(binary, instr_struct, instr_ptr);
      break;
    }
    case MULTIPLY: {
      multiply_t *instr_ptr = eMalloc(sizeof(multiply_t));
      multiply_helper(binary, instr_struct, instr_ptr);
      break;
    }
    case DATA_TRANSFER: {
      data_transfer_t *instr_ptr = eMalloc(sizeof(data_transfer_t));
      data_transfer_helper(binary, instr_struct, instr_ptr);
      break;
    }
    case BRANCH: {
      branch_t *instr_ptr = eMalloc(sizeof(branch_t));
      branch_helper(binary, instr_struct, instr_ptr);
      break;
    }
    default:
      break;
  }

  return instr_struct;
}

enum InstructionType check_instruction_type(const word_t binary) {
  /* check if it is the all-0 halt instruction */
  if (binary == 0) {
    return HALT;
  }

  /* the 26th(index starting with 0) bit of the unsigned integer binary */
  bool digit_26 = binary & (1 << 26);
  /* 27th(index starting with 0) bit of the unsigned integer binary */
  bool digit_27 = binary & (1 << 27);
  /* mul_flag is used to check instruction type Multiply.
   * If the 25th bit is 0 and both 4th and 7th are 1, it is guaranteed that the
   * instruction type is Multiply */
  bool mul_flag =
      !(binary & (1 << 25)) && (binary & (1 << 4)) && (binary & (1 << 7));

  if (digit_26) return DATA_TRANSFER;
  if (digit_27) return BRANCH;
  if (mul_flag) return MULTIPLY;
  if (!(digit_26 || digit_27)) return DATA_PROCESSING;

  exceptions(UNKNOWN_INSTRUCTION_TYPE, get_reg(PC));
  return EMPTY;
}

static void data_processing_helper(const word_t binary, instruction_t *struct_p,
                                   data_processing_t *instr_p) {
  instr_p->rn = (binary >> DP_DT_RN_LOCATION) & FOUR_BIT_FIELD;
  instr_p->rd = (binary >> DP_DT_RD_LOCATION) & FOUR_BIT_FIELD;
  instr_p->opcode = (binary >> OPCODE_LOCATION) & FOUR_BIT_FIELD;
  instr_p->imm_const = (binary >> IMM_LOCATION) & ONE_BIT_FIELD;
  instr_p->set = (binary >> SET_COND_LOCATION) & ONE_BIT_FIELD;

  /* Setting the operand2 field for DATA_PROCESSING instructions */
  if (instr_p->imm_const) {
    imm_value_t *imm_p = eMalloc(sizeof(imm_value_t));
    imm_p->imm = (binary >> OPERAND2_IMM_LOCATION) & EIGHT_BIT_FIELD;
    imm_p->rotate = (binary >> OPERAND2_ROTATE_LOCATION) & FOUR_BIT_FIELD;
    (instr_p->operand2).imm_value = imm_p;
  } else {
    register_form_t *reg_p = eMalloc(sizeof(register_form_t));
    reg_p->shift_type =
        (binary >> OPERAND2_SHIFT_TYPE_LOCATION) & TWO_BIT_FIELD;
    reg_p->rm = (binary >> OPERAND2_RM_LOCATION) & FOUR_BIT_FIELD;
    reg_p->shift_spec =
        (binary >> OPERAND2_SHIFT_SPEC_LOCATION) & ONE_BIT_FIELD;
    if (reg_p->shift_spec) {
      (reg_p->shift).shift_reg =
          (binary >> OPERAND2_REGISTER_SHIFT_LOCATION) & FOUR_BIT_FIELD;
    } else {
      (reg_p->shift).integer_shift =
          (binary >> OPERAND2_INTEGER_SHIFT_LOCATION) & FIVE_BIT_FIELD;
    }
    (instr_p->operand2).reg_value = reg_p;
  }

  (struct_p->instructions).data_processing = instr_p;
}

static void multiply_helper(const word_t binary, instruction_t *struct_p,
                            multiply_t *instr_p) {
  instr_p->rm = binary & FOUR_BIT_FIELD;
  instr_p->rd = (binary >> MUL_RD_LOCATION) & FOUR_BIT_FIELD;
  instr_p->rs = (binary >> MUL_RS_LOCATION) & FOUR_BIT_FIELD;
  instr_p->rn = (binary >> MUL_RN_LOCATION) & FOUR_BIT_FIELD;
  instr_p->accumulate = (binary >> ACCUMULATE_LOCATION) & ONE_BIT_FIELD;
  instr_p->set = (binary >> SET_COND_LOCATION) & ONE_BIT_FIELD;
  (struct_p->instructions).multiply = instr_p;
}

static void data_transfer_helper(const word_t binary, instruction_t *struct_p,
                                 data_transfer_t *instr_p) {
  instr_p->rd = (binary >> DP_DT_RD_LOCATION) & FOUR_BIT_FIELD;
  instr_p->rn = (binary >> DP_DT_RN_LOCATION) & FOUR_BIT_FIELD;
  instr_p->imm_offset = (binary >> IMM_LOCATION) & ONE_BIT_FIELD;
  instr_p->pre_index = (binary >> P_INDEX_LOCATION) & ONE_BIT_FIELD;
  instr_p->up_bit = (binary >> UP_BIT_LOCATION) & ONE_BIT_FIELD;
  instr_p->load = (binary >> LOAD_STORE_LOCATION) & ONE_BIT_FIELD;

  /* Setting the offset field for DATA_TRANSFER instructions */
  if (instr_p->imm_offset) {
    register_form_t *reg_p = eMalloc(sizeof(register_form_t));
    reg_p->shift_type =
        (binary >> OPERAND2_SHIFT_TYPE_LOCATION) & TWO_BIT_FIELD;
    reg_p->rm = (binary >> OPERAND2_RM_LOCATION) & FOUR_BIT_FIELD;
    reg_p->shift_spec =
        (binary >> OPERAND2_SHIFT_SPEC_LOCATION) & ONE_BIT_FIELD;
    if (reg_p->shift_spec) {
      (reg_p->shift).shift_reg =
          (binary >> OPERAND2_REGISTER_SHIFT_LOCATION) & FOUR_BIT_FIELD;
    } else {
      (reg_p->shift).integer_shift =
          (binary >> OPERAND2_INTEGER_SHIFT_LOCATION) & FIVE_BIT_FIELD;
    }
    (instr_p->offset).reg_value = reg_p;
  } else {
    (instr_p->offset).imm_value = binary & TWELVE_BIT_FIELD;
  }

  (struct_p->instructions).data_transfer = instr_p;
}

static void branch_helper(const word_t binary, instruction_t *struct_p,
                          branch_t *instr_p) {
  word_t raw_offset = binary & TWENTY_FOUR_BIT_FIELD;
  word_t offset = sign_extend(raw_offset << 2, 26);
  instr_p->offset = offset;
  (struct_p->instructions).branch = instr_p;
}
