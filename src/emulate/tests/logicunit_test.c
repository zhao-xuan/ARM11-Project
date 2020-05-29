#include "logicunit.h"

#include "global.h"
#include "testutils.h"

void test_and() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test and */
  name = "and: and can be computed correctly";
  op1 = 0b10011100011111100110100111001001;
  op2 = 0b110010100100101110101011;
  expected = 0b00000000010010100100100110001001;
  and(op1, op2, &carry, &result);
  testword(result, expected, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  and(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "and: and sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

void test_eor() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test eor */
  name = "eor: eor can be computed correctly";
  op1 = 0b10011100011111100110100111001001;
  op2 = 0b110010100100101110101011;
  expected = 0b10011100101101000010001001100010;
  eor(op1, op2, &carry, &result);
  testword(result, expected, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  eor(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "eor: eor sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

void test_sub() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test sub */
  name = "sub: sub can be computed correctly";
  op1 = 0b10011100011111100110100111001001;
  op2 = 0b110010100100101110101011;
  expected = 0b10011011101101000001111000011110;
  sub(op1, op2, &carry, &result);
  testword(result, expected, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  sub(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "sub: sub sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

void test_rsb() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test rsb */
  name = "rsb: rsb can be computed correctly";
  op1 = 0b10011100011111100110100111001001;
  op2 = 0b110010100100101110101011;
  expected = 0b01100100010010111110000111100010;
  rsb(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "rsb: rsb sets carry correctly";
  testbool(carry, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  rsb(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "rsb: rsb sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

void test_add() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test add */
  name = "add: add can be computed correctly";
  op1 = 0b10011100011111100110100111001001;
  op2 = 0b110010100100101110101011;
  expected = 0b10011101010010001011010101110100;
  add(op1, op2, &carry, &result);
  testword(result, expected, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  add(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "add: add sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

void test_orr() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test orr */
  name = "orr: orr can be computed correctly";
  op1 = 0b10011100011111100110100111001001;
  op2 = 0b110010100100101110101011;
  expected = 0b10011100111111100110101111101011;
  orr(op1, op2, &carry, &result);
  testword(result, expected, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  orr(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "orr: orr sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

void test_mov() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test mov */
  name = "mov: mov can be computed correctly";
  op1 = 0b10011100011111100110100111001001;
  op2 = 0b110010100100101110101011;
  expected = 0b110010100100101110101011;
  mov(op1, op2, &carry, &result);
  testword(result, expected, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  mov(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "mov: mov sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

void test_lsl() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test lsl */
  name = "lsl: lsl can be computed correctly";
  op1 = 4;
  op2 = 0b10100101100011010011010001101001;
  expected = 0b01011000110100110100011010010000;
  lsl(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "lsl: lsl sets carry correctly";
  testbool(!carry, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  lsl(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "lsl: lsl sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

void test_lsr() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test lsr */
  name = "lsr: lsr can be computed correctly";
  op1 = 4;
  op2 = 0b10100101100011010011010001101001;
  expected = 0b00001010010110001101001101000110;
  lsr(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "lsr: lsr sets carry correctly";
  testbool(carry, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  lsr(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "lsr: lsr sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

void test_asr() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test asr */
  name = "asr: asr can be computed correctly";
  op1 = 4;
  op2 = 0b00100101100011010011010001101001;
  expected = 0b00000010010110001101001101000110;
  asr(op1, op2, &carry, &result);
  testword(result, expected, name);

  op1 = 4;
  op2 = 0b10100101100011010011010001101001;
  expected = 0b11111010010110001101001101000110;
  asr(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "asr: asr sets carry correctly";
  testbool(carry, name);
  printf("\n");
}

void test_ror() {
  word_t op1, op2;
  word_t result, expected;
  bool carry;
  char *name; /* Test ror */
  name = "ror: ror can be computed correctly";
  op1 = 4;
  op2 = 0b010100101100011010011010001101001;
  expected = 0b10011010010110001101001101000110;
  ror(op1, op2, &carry, &result);
  testword(result, expected, name);
  op1 = 0;
  op2 = 0;
  expected = 0;
  ror(op1, op2, &carry, &result);
  testword(result, expected, name);
  name = "ror: ror sets carry correctly";
  testbool(!carry, name);
  printf("\n");
}

int main(void) {
  test_and();
  test_eor();
  test_sub();
  test_rsb();
  test_add();
  test_orr();
  test_mov();
  test_lsl();
  test_lsr();
  test_asr();
  test_ror();
  return 0;
}
