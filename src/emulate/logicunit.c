#include "logicunit.h"


/*
 *  The following functions define require ALU operations
 *  required for Data-Processing-Instructions for part 1
 *  
 *  @param: 
 *    op1, op2: the 2 operands 
 *    cout: pointer to write the carry-out bit into
 *    res: pointer to where to write the result into
 *  
 *    
 *  NOTICE that this Carryout is ONLY responsible for the C-bit
 *  of the instruction currently being executed
 */
static void and(word_t op1, word_t op2, bool *cout, word_t *res);
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


typedef void ( *alu_array_ptr ) ( word_t, word_t, bool *, word_t *);

typedef void ( *shifter_array_ptr ) ( byte_t, word_t, bool *, word_t *);

typedef void ( *flag_setter_ptr ) (flag_t flag);

/* Array of pointer to ALU functions */
alu_array_ptr alu_selector[] = { and, eor, sub, rsb, add, orr, mov };

/* Array of pointer to Barrel shifter functions */
shifter_array_ptr barrel_shifter[] = { lsl, lsr, asr, ror };

/* Array of pointer to set flags function */
flag_setter_ptr flag_setter[] = { clear_flag, set_flag };

/* A helper function to set the flags of the top 3 bits in CPSR*/
static void set_alu_flags(word_t res, bool cout) {
    flag_setter[(res >> 31) & 1] (N_FLAG);
    flag_setter[res == 0] (Z_FLAG);
    flag_setter[cout] (C_FLAG);
}


/* A function visible to the 'outside world' defined in the header file */
int alu(word_t op1, word_t op2, word_t *result, byte_t opcode, bool set) {
  if ((opcode >= 5 && opcode <= 7) || opcode == 11 || opcode > 13) {
    return UNKNOWN_OPCODE;
  }
  bool cout;
  int index = opcode % 8 + (opcode > 10 ? 1 : 0);
  alu_selector[index](op1, op2, &cout, result);
  if (set) set_alu_flags(*result, cout); 
  return 0;
}

/* A function visible to the 'outside world' for the barrel shifter */
int shifter(word_t op1, word_t op2, word_t *result, byte_t shift_type, bool set) {
  if (shift_type > 4) return UNKNOWN_OPCODE;
  bool cout;
  barrel_shifter[shift_type](op1, op2, &cout, result);
  if (set) set_alu_flags(*result, cout); 
  return 0;
}

/*  BITWISE AND */
static void and(word_t op1, word_t op2, bool *cout, word_t *res){
  *cout = false;
  *res = op1 & op2;
}

/*  BITWISE EXCLUSTIVE OR */
static void eor(word_t op1, word_t op2, bool *cout, word_t *res){
  *cout = false;
  *res = op1 ^ op2;
}


/*  SUBTRACTION */
static void sub(word_t op1, word_t op2, bool *cout, word_t *res){
  *res = op1 - op2;  
  int sign_op1 = op1 >> 31;
  int sign_op2 = op2 >> 31;
  int sign_diff = *res >> 31;
  *cout = (sign_op1 ^ sign_op2) && !(sign_diff & sign_op2);    
}


/*  REVERSE SUBTRACTION */
static void rsb(word_t op1, word_t op2, bool *cout, word_t *res){
   sub(op2, op1, cout, res);
}

/*  ADDITION */
static void add(word_t op1, word_t op2, bool *cout, word_t *res){
  int32_t signed_op1 = (int32_t) op1;
  int32_t signed_op2 = (int32_t) op2;
  *res = (word_t) (signed_op1 + signed_op2);
  bool overflow = signed_op1 > 0 && (signed_op2 > INT_MAX - signed_op1);
  bool underflow = signed_op1 < 0 && (signed_op2 < INT_MAX - signed_op1);
  *cout = overflow || underflow;
}


/*  BITWISE */
static void orr(word_t op1, word_t op2, bool *cout, word_t *res){
  *res = op1 | op2;
  *cout = false;  
}



/*  MOVE */
static void mov(word_t op1, word_t op2, bool *cout, word_t *res){
 *res = op2;
 *cout = false;
}


/*
 *  @param: shiftamount and number
 *  @return: 32-bit unsigned integer (word_t)
 */
static void lsl(byte_t shamt, word_t n, bool *cout, word_t *res){
  *cout = (n >> (32 - shamt)) & 1;
  *res =  n << shamt;
}

static void lsr(byte_t shamt, word_t n, bool *cout, word_t *res){
  *cout = (n >> (shamt - 1)) & 1;
  *res =  n >> shamt;
}

static void asr(byte_t shamt, word_t n, bool *cout, word_t *res){
  *cout = shamt > 0 && (n >> (shamt - 1));
  n >>= shamt;
  word_t mask = 1 << (31 - shamt);
  word_t trailing_ones = (n & mask) - 1;
  *res =  ~trailing_ones | n ;
}

static void ror(byte_t shamt, word_t n, bool *cout, word_t *res){
  *res = (n >> shamt) | (n << (-shamt & 31));
  *cout = (*res >> 31) & 1;
}
