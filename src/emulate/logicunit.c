/*
 * Implementation of the ALU and barrel shifter operations.
 */

#include <limits.h>
#include "logicunit.h"

static void and (word_t op1, word_t op2, bool *cout, word_t *res);
static void eor(word_t op1, word_t op2, bool *cout, word_t *res);
static void sub(word_t op1, word_t op2, bool *cout, word_t *res);
static void rsb(word_t op1, word_t op2, bool *cout, word_t *res);
static void add(word_t op1, word_t op2, bool *cout, word_t *res);
static void orr(word_t op1, word_t op2, bool *cout, word_t *res);
static void mov(word_t op1, word_t op2, bool *cout, word_t *res);

static void lsl(byte_t shamt, word_t op2, bool *cout, word_t *res);
static void lsr(byte_t shamt, word_t op2, bool *cout, word_t *res);
static void asr(byte_t shamt, word_t op2, bool *cout, word_t *res);
static void ror(byte_t shamt, word_t op2, bool *cout, word_t *res);
static void set_alu_flags(word_t res, bool cout, bool arithm_op);

typedef void (*alu_array_ptr)(word_t, word_t, bool *, word_t *);

typedef void (*shifter_array_ptr)(byte_t, word_t, bool *, word_t *);

typedef void (*flag_setter_ptr)(flag_t flag);

/* Array of pointer to ALU functions */
alu_array_ptr alu_selector[] = {and, eor, sub, rsb, add, orr, mov};

/* Array of pointer to barrel shifter functions */
shifter_array_ptr barrel_shifter[] = {lsl, lsr, asr, ror};

/* Array of pointer to set flags function */
flag_setter_ptr flag_setter[] = {clear_flag, set_flag};

int alu(word_t op1, word_t op2, word_t *result, byte_t opcode, bool set)
{
  if ((opcode >= 5 && opcode <= 7) || opcode == 11 || opcode > 13)
  {
    return UNKNOWN_OPCODE;
  }
  bool cout;
  int index = opcode % 8 + (opcode > 10 ? 1 : 0);
  alu_selector[index](op1, op2, &cout, result);
  bool arithm_op = (opcode >= 2 && opcode <= 4) || opcode == 10;
  if (set)
    set_alu_flags(*result, cout, arithm_op);
  return 0;
}

int shifter(byte_t shamt, word_t operand, word_t *result,
            byte_t shift_type, bool set)
{
  if (shift_type > 4)
    return UNKNOWN_OPCODE;
  bool cout;
  barrel_shifter[shift_type](shamt % 32, operand, &cout, result);
  if (set)
    set_alu_flags(*result, cout, true);
  return 0;
}

/*
 * ALU operations
 * @param op1: operand 1
 * @param op2: operand 2
 * @param *cout: a pointer to the carry out value
 * @param *res: a pointer to the result
 */

/* Bitwise AND */
static void and (word_t op1, word_t op2, bool *cout, word_t *res)
{
  *res = op1 & op2;
}

/* Bitwise EXCLUSTIVE OR */
static void eor(word_t op1, word_t op2, bool *cout, word_t *res)
{
  *res = op1 ^ op2;
}

/* SUBTRACTION */
static void sub(word_t op1, word_t op2, bool *cout, word_t *res)
{
  *res = op1 - op2;
  *cout = op1 >= op2;
}

/* Reverse SUBTRACTION */
static void rsb(word_t op1, word_t op2, bool *cout, word_t *res)
{
  sub(op2, op1, cout, res);
}

/* Addition */
static void add(word_t op1, word_t op2, bool *cout, word_t *res)
{
  int32_t signed_op1 = (int32_t)op1;
  int32_t signed_op2 = (int32_t)op2;
  *res = (word_t)(signed_op1 + signed_op2);
  bool overflow = signed_op1 > 0 && (signed_op2 > INT_MAX - signed_op1);
  bool underflow = signed_op1 < 0 && (signed_op2 < INT_MAX - signed_op1);
  *cout = overflow || underflow;
}

/* Bitwise OR */
static void orr(word_t op1, word_t op2, bool *cout, word_t *res)
{
  *res = op1 | op2;
}

/* MOVE */
static void mov(word_t op1, word_t op2, bool *cout, word_t *res)
{
  *res = op2;
}

/*
 * Barrel shifter operations.
 * @param shamt: shift amount.
 * @param n: the input value.
 * @param *cout: a pointer to the carry out value.
 * @param *res: a pointer to the result value.
 */
static void lsl(byte_t shamt, word_t n, bool *cout, word_t *res)
{
  *cout = (n >> (31 - shamt)) & 1;
  *res = n << shamt;
}

static void lsr(byte_t shamt, word_t n, bool *cout, word_t *res)
{
  *cout = (n >> (shamt - 1)) & 1;
  *res = n >> shamt;
}

static void asr(byte_t shamt, word_t n, bool *cout, word_t *res)
{
  *cout = shamt > 0 && (n >> (shamt - 1) & 1);
  n >>= shamt;
  word_t mask = 1 << (31 - shamt);
  word_t trailing_ones = (n & mask) - 1;
  *res = ~trailing_ones | n;
}

static void ror(byte_t shamt, word_t n, bool *cout, word_t *res)
{
  *cout = shamt > 0 && (n >> (shamt - 1) & 1);
  *res = (n >> shamt) | (n << (-shamt & 31));
}

/*
 * A helper function to set the flags of the top 3 bits in CPSR registers.
 */
static void set_alu_flags(word_t res, bool cout, bool arithm_op)
{
  flag_setter[(res >> 31) & 1](N_FLAG);
  flag_setter[res == 0](Z_FLAG);
  if (arithm_op)
    flag_setter[cout](C_FLAG);
}
