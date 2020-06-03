/*
 * Unit tests for logicunit.c
 * Run ./logicunit_test
 */

#include "logicunit.h"

#include <limits.h>
#include <string.h>

#include "global.h"
#include "state.h"
#include "testutils.h"

/* Give the name of the test from the opcode */
char *test_name(int opcode) {
  switch (opcode) {
    case AND_OPCODE:
      return "bitwise and ";
    case EOR_OPCODE:
      return "bitwise exclusive or ";
    case SUB_OPCODE:
      return "arithmetic subtraction ";
    case RSB_OPCODE:
      return "reversed arithmetic subtraction ";
    case ADD_OPCODE:
      return "arithmetic addition ";
    case ORR_OPCODE:
      return "bitwise or ";
    case MOV_OPCODE:
      return "move ";
    default:
      return "Unknown test ";
  }
}

/* Given the name of the test from the shift_type */
char *shift_name(int shift_type) {
  switch (shift_type) {
    case LSL_OPCODE:
      return "logical shift left ";
    case LSR_OPCODE:
      return "logical shift right ";
    case ASR_OPCODE:
      return "arithmetic shift right ";
    case ROR_OPCODE:
      return "rotate right ";
    default:
      return "Unknown test ";
  }
}

/*
 * Return the current flag status. Encoded in binary
 * 0b1010 refers to NC flags being set and ZV flags not
 */
int get_flags() {
  return (get_flag(N_FLAG) << 3) + (get_flag(Z_FLAG) << 2) +
         (get_flag(C_FLAG) << 1) + get_flag(V_FLAG);
}

void test_alu(word_t op1, word_t op2, int opcode, word_t expected,
              int expected_flags) {
  char name[100];
  sprintf(name, "ALU: %scan be computed correctly", test_name(opcode));
  word_t result;
  alu(op1, op2, &result, opcode, true);
  testword(result, expected, name);

  sprintf(name, "ALU: %ssets the flags correctly", test_name(opcode));
  testint(get_flags(), expected_flags, name);
}

void test_shifter(word_t op1, word_t op2, int shift_type, word_t expected) {
  char name[100];
  sprintf(name, "Shifter: %scan be computed correctly", shift_name(shift_type));
  word_t result;
  shifter(op1, op2, &result, shift_type, false);
  testword(result, expected, name);
}

void test_combined(word_t op1, word_t op2, word_t shamt, int shift_type,
                   int opcode, word_t expected, int expected_flags) {
  char name[150];
  sprintf(name, "Combined: %s-> %scan be computed correctly",
          shift_name(shift_type), test_name(opcode));
  word_t partial, result;
  shifter(shamt, op2, &partial, shift_type, true);
  alu(op1, partial, &result, opcode, true);
  testword(result, expected, name);

  sprintf(name, "Combined: %s-> %ssets the flags correctly",
          shift_name(shift_type), test_name(opcode));
  testint(get_flags(), expected_flags, name);
}

int main(void) {
  init_state();

  word_t num_max = UINT_MAX;
  word_t num0 = 0;
  /* 0000_0000_0000_0000_1111_1111_1111_1111 */
  word_t num1 = 65535;
  /* 1111_1111_1111_1111_0000_0000_0000_0000 */
  word_t num2 = 4294901760;
  /* 1000_1101_1011_1100_0001_1010_0011_0010 */
  word_t num3 = 2377914930;
  /* 0011_1011_0001_0110_1001_1001_1011_0100 */
  word_t num4 = 991336884;
  /* 1101_0010_0011_1011_0011_0001_0000_1011 */
  word_t num5 = 3527094539;
  word_t expected;
  int expected_flags, shamt;

  clear_flag(C_FLAG);
  /* Test bitwise and */
  expected = 0;
  /* 0b0100 */
  expected_flags = 4;
  test_alu(num1, num2, AND_OPCODE, expected, expected_flags);
  expected = num3;
  /* 0b1000 */
  expected_flags = 8;
  test_alu(num_max, num3, AND_OPCODE, expected, expected_flags);
  printf("\n");

  /* Test bitwise exclusive or */
  expected = 0;
  /* 0b0100 */
  expected_flags = 4;
  test_alu(num0, num0, EOR_OPCODE, expected, expected_flags);
  /* 0111_0010_0100_0011_0001_1010_0011_0010 */
  expected = 1917000242;
  /* 0b0000 */
  expected_flags = 0;
  test_alu(num2, num3, EOR_OPCODE, expected, expected_flags);
  printf("\n");

  /* Test arithmetic subtraction */
  expected = 1;
  /* 0b0000 */
  expected_flags = 0;
  test_alu(num0, num_max, SUB_OPCODE, expected, expected_flags);
  expected = 0;
  /* 0b0110 */
  expected_flags = 6;
  test_alu(num0, num0, SUB_OPCODE, expected, expected_flags);
  expected = 2535757655;
  /* 0b1010 */
  expected_flags = 10;
  test_alu(num5, num4, SUB_OPCODE, expected, expected_flags);
  printf("\n");

  /* Test reversed arithmetic subtraction */
  expected = -1916986830;
  /* 0b1000 */
  expected_flags = 8;
  test_alu(num2, num3, RSB_OPCODE, expected, expected_flags);
  printf("\n");

  /* Test arithmetic addition */
  expected = num_max;
  /* 0b1000 */
  expected_flags = 8;
  test_alu(num1, num2, ADD_OPCODE, expected, expected_flags);
  expected = 0;
  /* 0b0100 */
  expected_flags = 4;
  test_alu(num0, num0, ADD_OPCODE, expected, expected_flags);
  expected = 1610042173;
  /* 0b0010 */
  expected_flags = 2;
  test_alu(num3, num5, ADD_OPCODE, expected, expected_flags);
  printf("\n");

  clear_flag(C_FLAG);
  /* Test bitwise or */
  expected = num_max;
  /* 0b1000 */
  expected_flags = 8;
  test_alu(num1, num2, ORR_OPCODE, expected, expected_flags);
  /* 1011_1111_1011_1110_1001_1011_1011_0110 */
  expected = 3216939958;
  /* 0b1000 */
  expected_flags = 8;
  test_alu(num3, num4, ORR_OPCODE, expected, expected_flags);
  printf("\n");

  /* Test move */
  expected = num5;
  /* 0b1000 */
  expected_flags = 8;
  test_alu(num0, num5, MOV_OPCODE, expected, expected_flags);

  /* Test set flag */
  clear_flag(N_FLAG);
  set_flag(Z_FLAG);
  set_flag(C_FLAG);
  clear_flag(V_FLAG);
  /* 0b0110 */
  expected_flags = 6;
  word_t dummy;
  alu(0, 0, &dummy, ADD_OPCODE, false);
  char *name = "ALU: doesn't update flags with condition codes unset";
  testint(get_flags(), expected_flags, name);
  printf("\n");

  /* Test logical shift left */
  shamt = 5;
  expected = 1197875552;
  test_shifter(shamt, num5, LSL_OPCODE, expected);

  /* Test logical shift right */
  shamt = 16;
  expected = num1;
  test_shifter(shamt, num2, LSR_OPCODE, expected);
  shamt = 10;
  expected = 0;
  test_shifter(shamt, num0, LSR_OPCODE, expected);

  /* Test arithmetic shift right */
  shamt = 16;
  expected = 0;
  test_shifter(shamt, num1, ASR_OPCODE, expected);
  shamt = 8;
  /* 1111_1111_1000_1101_1011_1100_0001_1010 */
  expected = 4287478810;
  test_shifter(shamt, num3, ASR_OPCODE, expected);

  /* Test rorate right */
  shamt = 30;
  expected = num_max;
  test_shifter(shamt, num_max, ROR_OPCODE, expected);
  shamt = 24;
  /* 0001_0110_1001_1001_1011_0100_0011_1011 */
  expected = 379171899;
  test_shifter(shamt, num4, ROR_OPCODE, expected);
  printf("\n");

  /* Test logical shift left -> bitwise or */
  shamt = 8;
  /* 1111_1111_0000_0000_1111_1111_1111_1111 */
  expected = 4278255615;
  /* 0b1010 */
  expected_flags = 10;
  test_combined(num1, num2, shamt, LSL_OPCODE, ORR_OPCODE, expected,
                expected_flags);

  /* Test logical shift right -> arithmetic addition */
  shamt = 8;
  /* 0000_0000_0011_1011_0001_0110_1001_1001 */
  expected = 3872409;
  /* 0b0000 */
  expected_flags = 0;
  test_combined(num0, num4, shamt, LSR_OPCODE, ADD_OPCODE, expected,
                expected_flags);

  /* Test arithmetic shift right -> bitwise and */
  shamt = 16;
  /* 1111_1111_1111_1111_1000_1101_1011_1100 */
  expected = 4294938044;
  /* 0b1000 */
  expected_flags = 8;
  test_combined(num_max, num3, shamt, ASR_OPCODE, AND_OPCODE, expected,
                expected_flags);

  /* Test rotate right -> exclusive or */
  shamt = 8;
  /* 0110_0110_0000_0000_0010_0111_1001_0010 */
  expected = 1711286162;
  /* 0b0010 */
  expected_flags = 2;
  test_combined(num5, num4, shamt, ROR_OPCODE, EOR_OPCODE, expected,
                expected_flags);

  free_state();
  return 0;
}
