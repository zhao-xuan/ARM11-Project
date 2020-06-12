/*
 * Unit tests for decode.c
 * Run ./decode_test
 */

#include "decode.h"

#include <stdbool.h>

#include "global.h"
#include "state.h"
#include "testutils.h"

static bool compare_imm_value(imm_value_t *expected, imm_value_t *output) {
  return expected->imm == output->imm && expected->rotate == output->rotate;
}

static bool compare_register_form(register_form_t *expected,
                                  register_form_t *output) {
  if (expected->shift_spec != output->shift_spec) return false;

  bool shift_val_match =
      expected->shift_spec
          ? expected->shift.shift_reg == output->shift.shift_reg
          : expected->shift.integer_shift == output->shift.integer_shift;

  return shift_val_match && expected->shift_type == output->shift_type &&
         expected->rm == output->rm;
}

static bool compare_data_processing(data_processing_t *expected,
                                    data_processing_t *output) {
  if (expected->imm_const ^ output->imm_const) return false;
  bool operand2_decoded_correctly =
      expected->imm_const ? compare_imm_value(expected->operand2.imm_value,
                                              output->operand2.imm_value)
                          : compare_register_form(expected->operand2.reg_value,
                                                  output->operand2.reg_value);
  return operand2_decoded_correctly && expected->rn == output->rn &&
         expected->rd == output->rd && expected->opcode == output->opcode &&
         expected->set == output->set;
}
static bool compare_multiply(multiply_t *expected, multiply_t *output) {
  return expected->rm == output->rm && expected->rd == output->rd &&
         expected->rs == output->rs &&
         expected->accumulate == output->accumulate &&
         expected->set == output->set;
}
static bool compare_data_transfer(data_transfer_t *expected,
                                  data_transfer_t *output) {
  if (expected->imm_offset ^ output->imm_offset) return false;
  bool offset_decoded_correctly =
      expected->imm_offset
          ? compare_register_form(expected->offset.reg_value,
                                  output->offset.reg_value)
          : expected->offset.imm_value == output->offset.imm_value;

  return offset_decoded_correctly && expected->rn == output->rn &&
         expected->rd == output->rd &&
         expected->imm_offset == output->imm_offset &&
         expected->pre_index == output->pre_index &&
         expected->up_bit == output->up_bit && expected->load == output->load;
}
static bool compare_branch(branch_t *expected, branch_t *output) {
  return expected->offset == output->offset;
}

static bool compare_instr(instruction_t *expected, instruction_t *output) {
  if (expected->type != output->type || expected->cond != output->cond)
    return false;
  switch (expected->type) {
    case DATA_PROCESSING:
      return compare_data_processing(expected->instructions.data_processing,
                                     output->instructions.data_processing);
    case MULTIPLY:
      return compare_multiply(expected->instructions.multiply,
                              output->instructions.multiply);
    case DATA_TRANSFER:
      return compare_data_transfer(expected->instructions.data_transfer,
                                   output->instructions.data_transfer);
    case BRANCH:
      return compare_branch(expected->instructions.branch,
                            output->instructions.branch);
    default:
      return true;
  }
}

char *generate_test_name(instruction_t *expected) {
  switch (expected->type) {
    case DATA_PROCESSING: {
      data_processing_t *dp_ptr = expected->instructions.data_processing;
      if (dp_ptr->imm_const) {
        return "Data Processing instruction with immediate constant, decoded "
               "correctly";
      }
      return "Data Processing instruction with register form, decoded "
             "correctly";
    }
    case MULTIPLY:
      return "Multiply instruction decoded correctly";
    case DATA_TRANSFER: {
      data_transfer_t *dt_ptr = expected->instructions.data_transfer;
      if (dt_ptr->imm_offset) {
        return "Data Transfer instruction with register form, decoded "
               "correctly";
      }
      return "Data Transfer instruction with immediate constant, decoded "
             "correctly";
    }
    case BRANCH:
      return "Branch instruction decoded correctly";
    default:
      return "Halt instruction decoded correctly";
  }
}

static void print_data_processing(data_processing_t *instr) {
  printf("rn       : %u\n", instr->rn);
  printf("rd       : %u\n", instr->rd);
  printf("opcode   : %u\n", instr->opcode);
  printf("imm_const: %u\n", instr->imm_const);
  printf("set      : %u\n", instr->set);

  if (instr->imm_const) {
    imm_value_t *imm_p = instr->operand2.imm_value;
    printf("imm_value: %u\n", imm_p->imm);
    printf("shamt    : %u\n", imm_p->rotate);
  } else {
    register_form_t *reg_p = instr->operand2.reg_value;
    printf("shifttype: %u\n", reg_p->shift_type);
    printf("rm       : %u\n", reg_p->rm);
    printf("shiftspec: %u\n", reg_p->shift_spec);
    if (reg_p->shift_spec) {
      printf("int shift : %u\n", reg_p->shift.integer_shift);
    } else {
      printf("shift reg : %u\n", reg_p->shift.shift_reg);
    }
  }
}

static void print_data_transfer(data_transfer_t *instr) {
  printf("imm_offset: %u\n", instr->imm_offset);
  printf("rn        : %u\n", instr->rn);
  printf("rd        : %u\n", instr->rd);
  printf("pre_index : %u\n", instr->pre_index);
  printf("up_bit    : %u\n", instr->up_bit);
  printf("load      : %u\n", instr->load);

  if (instr->imm_offset) {
    register_form_t *reg_p = instr->offset.reg_value;
    printf("shifttype : %u\n", reg_p->shift_type);
    printf("rm        : %u\n", reg_p->rm);
    printf("shiftspec : %u\n", reg_p->shift_spec);
    if (reg_p->shift_spec) {
      printf("int shift : %u\n", reg_p->shift.integer_shift);
    } else {
      printf("shift reg : %u\n", reg_p->shift.shift_reg);
    }
  } else {
    printf("offset imm : %u\n", instr->offset.imm_value);
  }
}

static void print_branch(branch_t *instr) {
  printf("offset: %u\n", instr->offset);
}

static void print_multiply(multiply_t *instr) {
  printf("rm        : %u\n", instr->rm);
  printf("rd        : %u\n", instr->rd);
  printf("rs        : %u\n", instr->rs);
  printf("rn        : %u\n", instr->rn);
  printf("accumulate: %u\n", instr->accumulate);
  printf("set       : %u\n", instr->set);
}

static void print_decoded(instruction_t *instr) {
  printf("cond: %u\n", instr->cond);
  switch (instr->type) {
    case DATA_PROCESSING:
      print_data_processing(instr->instructions.data_processing);
      break;
    case MULTIPLY:
      print_multiply(instr->instructions.multiply);
      break;
    case DATA_TRANSFER:
      print_data_transfer(instr->instructions.data_transfer);
      break;
    case BRANCH:
      print_branch(instr->instructions.branch);
      break;
    default:
      break;
  }
}

void test_instruction(instruction_t *expected, const word_t binary) {
  instruction_t *got = decode(binary);
  char instr_string[30];
  sprintf(instr_string, "\n  Instr in hex: 0x%08x", binary);
  char *partial_test_name = generate_test_name(expected);
  bool test_result = compare_instr(expected, got);
  char test_name[200];
  strcpy(test_name, partial_test_name);
  strcat(test_name, instr_string);
  testbool(test_result, test_name);
  if (!test_result) {
    printf("EXPECTED: \n");
    print_decoded(expected);
    printf("GOT: \n");
    print_decoded(got);
  }
}

int main() {
  word_t input, imm;
  data_processing_t dp;
  imm_value_t imm_value;
  register_form_t reg_value;
  multiply_t mul;
  data_transfer_t dt;
  branch_t branch;
  instruction_t expected;

  /*
   *  Data Processing with immediate constant
   *  add01
   *  11100011 10100000 00010000 00000001
   */
  input = 3818917889;
  expected.type = DATA_PROCESSING;
  expected.cond = 14;
  imm_value = (imm_value_t){1, 0};
  dp = (data_processing_t){0, 1, 13, 1, 0};
  dp.operand2.imm_value = &imm_value;
  expected.instructions.data_processing = &dp;
  test_instruction(&expected, input);

  /*
   *  Data Processing with register form
   *  add04
   *  11100000 10000001 00110000 00000010
   */
  input = 3766562818;
  expected.type = DATA_PROCESSING;
  expected.cond = 14;
  reg_value = (register_form_t){0, 2};
  reg_value.shift.integer_shift = 0;
  reg_value.shift_spec = 0;
  dp = (data_processing_t){1, 3, 4, 0, 0};
  dp.operand2.reg_value = &reg_value;
  expected.instructions.data_processing = &dp;
  test_instruction(&expected, input);

  /*
   *  Data Processing with immediate constatnt
   *  eor1
   *  11100011 10100000 00100000 11111111
   */
  input = 3818922239;
  expected.type = DATA_PROCESSING;
  expected.cond = 14;
  imm_value = (imm_value_t){0xff, 0};
  dp = (data_processing_t){0, 2, 13, 1, 0};
  dp.operand2.imm_value = &imm_value;
  expected.instructions.data_processing = &dp;
  test_instruction(&expected, input);

  /*  Multiply
   *  mul01
   *  11100000 00000011 00000010 10010001
   */
  input = 3758293649;
  expected.type = MULTIPLY;
  expected.cond = 14;
  mul = (multiply_t){1, 3, 2, 0, 0, 0};
  expected.instructions.multiply = &mul;
  test_instruction(&expected, input);

  /* Data transfer with immediate constant
   *  ldr04
   *  11100101 10010000 00100000 00000000
   */
  input = 3851427840;
  expected.cond = 14;
  expected.type = DATA_TRANSFER;
  imm = 0;
  dt = (data_transfer_t){0, 2, 0, 1, 1, 1};
  dt.offset.imm_value = imm;
  expected.instructions.data_transfer = &dt;
  test_instruction(&expected, input);

  /* Data transfer with register form
   *  str02
   *  11100110 10000010 00010000 00000100
   */
  input = 3867283460;
  expected.cond = 14;
  expected.type = DATA_TRANSFER;
  reg_value.shift_type = 0;
  reg_value.rm = 4;
  reg_value.shift_spec = 0;
  dt = (data_transfer_t){2, 1, 1, 0, 1, 0};
  dt.offset.reg_value = &reg_value;
  expected.instructions.data_transfer = &dt;
  test_instruction(&expected, input);
  /* Branch
   *  b01
   *  11101010 00000000 00000000 00000000
   */
  input = 3925868544;
  expected.cond = 14;
  expected.type = BRANCH;
  branch = (branch_t){0};
  expected.instructions.branch = &branch;
  test_instruction(&expected, input);

  /*  Branch with negative sign extend
   *  Modified b01
   *  11101010 10000000 11111111 00000000
   */
  input = 3934322432;
  expected.cond = 14;
  expected.type = BRANCH;
  branch = (branch_t){4261673984};
  expected.instructions.branch = &branch;
  test_instruction(&expected, input);

  /* Halt */
  input = 0;
  expected.cond = 0;
  expected.type = HALT;
  test_instruction(&expected, input);
  return 0;
}
