#include "logicunit.h"
#include "global.h"
#include "testutils.h"
#include "state.h"

/* Give the name of the test from the opcode */
char *test_name(int opcode) {
  switch(opcode) {
    case AND_OPCODE:
      return "ALU: bitwise and ";
    case EOR_OPCODE:
      return "ALU: bitwise exclusive or ";
    default:
      return "Unknown test ";
  }
}

/* Return the current flag status. Encoded in binary
 * 0b1010 refers to NC flags being set and ZV flags not
 */
int get_flags() {
  return (get_flag(N_FLAG) << 3) + (get_flag(Z_FLAG) << 2)
    + (get_flag(C_FLAG) << 1) + get_flag(V_FLAG);
}

void test_alu(word_t op1, word_t op2, int opcode, word_t expected, int expected_flags) {
  char name[100];
  strcpy(name, test_name(opcode));
  strcat(name, "can be computed correctly");
  word_t result;
  alu(op1, op2, &result, opcode, true);
  testword(result, expected, name);

  strcpy(name, test_name(opcode));
  strcat(name, "sets the flags correctly");
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
  int expected_flags;

  /* Test bitwise and */
  expected = 0;
  expected_flags = 0b0100;
  test_alu(num1, num2, AND_OPCODE, expected, expected_flags);
  expected = num3;
  expected_flags = 0b1000;
  test_alu(num_max, num3, AND_OPCODE, expected, expected_flags);
  printf("\n");

  /* Test bitwise exclusive or */
  expected = 0;
  expected_flags = 0b0100;
  test_alu(num0, num0, EOR_OPCODE, expected, expected_flags);
  /* 0111_0010_0100_0011_0001_1010_0011_0010 */
  expected = 1917000242;
  expected_flags = 0b0000;
  test_alu(num2, num3, EOR_OPCODE, expected, expected_flags);
  printf("\n");

  return 0;
}
