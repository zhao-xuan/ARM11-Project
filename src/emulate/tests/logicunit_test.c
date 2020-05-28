#include "global.h"
#include "testutils.h"
#include "logicunit.h"

int main(void) {
  int op1, op2;
  word_t result, expected;
  bool carry;
  char *name;

  
  /* Test bitwise and */
  name = "and: bitwise and can be computed correctly";
  op1 =      0b10011100011111100110100111001001;
  op2 =              0b110010100100101110101011;
  expected = 0b00000000010010100100100110001001;
  and(op1, op2, &carry, &result);
  testword(result, expected, name);

  op1 = 0;
  op2 = 0;
  expected = 0;
  and(op1, op2, &carry, &result);
  testword(result, expected, name);

  name = "and: bitwise and sets carry correctly";
  testbool(!carry, name);
  printf("\n");


  return 0;
}
