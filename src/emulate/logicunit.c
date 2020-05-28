#include "logicunit.h"

/* Array of pointer to ALU functions */
void (*alu_selector[7]) (word_t op1, word_t op2, bool *cout, word_t *res) = 
  { add, eor, sub, rsb, add, orr, mov }; 

/* Array of pointer to Barrel shifter functions */
void (*barrel_shifter[4]) (byte_t op1, word_t op2, bool *cout, word_t *res) =
  { lsl, lsr, asr, ror };

/* A helper function to set the flags of the top 3 bits in CPSR*/
void set_nzc_flags(bool neg, bool zero, bool cout) {
  word_t newflag = neg << 31 | zero << 30 | cout << 29; 
  word_t oldflag = get_reg(CPSR) & ((1 << 29) - 1);
  set_reg(CPSR, newflag | oldflag);
}


/* A function visible to the 'outside world' defined in the header file */
int alu(word_t op1, word_t op2, byte_t opcode, bool set) {
  if ((opcode >= 5 && opcode <= 7) || opcode == 11 || opcode > 13) {
    return UNKNOWN_OPCODE;
  }
  word_t res;
  bool cout;
  int index = opcode % 8 + (opcode > 10 ? 1 : 0);
  (*alu_selector[index]) (op1, op2, &cout, &res);
  if (set) set_nzc_flags((res >> 31) & 1, res == 0, cout); 
  return 0;
}

/* A function visible to the 'outside world' for the barrel shifter */
int shifter(word_t op1, word_t op2, byte_t shift_type, bool set) {
  if (shift_type > 4) return UNKNOWN_OPCODE;
  word_t res;
  bool cout;
  (*barrel_shifter[shift_type])(op1, op2, &cout, &res);
  if (set) set_nzc_flags((res >> 31) & 1, res == 0, cout); 
  return 0;
}

/*  BITWISE AND */
void and(word_t op1, word_t op2, bool *cout, word_t *res){
  *cout = false;
  *res = op1 & op2;
}

/*  BITWISE EXCLUSTIVE OR */
void eor(word_t op1, word_t op2, bool *cout, word_t *res){
  *cout = false;
  *res = op1 ^ op2;
}


/*  SUBTRACTION */
void sub(word_t op1, word_t op2, bool *cout, word_t *res){
  *res = op1 - op2;  
  int sign_op1 = op1 >> 31;
  int sign_op2 = op2 >> 31;
  int sign_diff = *res >> 31;
  *cout = (sign_op1 ^ sign_op2) && !(sign_diff & sign_op2);    
}


/*  REVERSE SUBTRACTION */
void rsb(word_t op1, word_t op2, bool *cout, word_t *res){
   sub(op2, op1, cout, res);
}

/*  ADDITION */
void add(word_t op1, word_t op2, bool *cout, word_t *res){
  int32_t signed_op1 = (int32_t) op1;
  int32_t signed_op2 = (int32_t) op2;
  *res = (word_t) (signed_op1 + signed_op2);
  bool overflow = signed_op1 > 0 && (signed_op2 > INT_MAX - signed_op1);
  bool underflow = signed_op1 < 0 && (signed_op2 < INT_MAX - signed_op1);
  *cout = overflow || underflow;
}


/*  BITWISE */
void orr(word_t op1, word_t op2, bool *cout, word_t *res){
  *res = op1 | op2;
}


/*  MOVE */
void mov(word_t op1, word_t op2, bool *cout, word_t *res){
 *res = op2;
}


/*
 *  @param: shiftamount and number
 *  @return: 32-bit unsigned integer (word_t)
 */
void lsl(byte_t shamt, word_t n, bool *cout, word_t *res){
  *cout = (n >> (32 - shamt)) & 1;
  *res =  n << shamt;
}

void lsr(byte_t shamt, word_t n, bool *cout, word_t *res){
  *cout = (n >> (shamt - 1)) & 1;
  *res =  n >> shamt;
}

void asr(byte_t shamt, word_t n, bool *cout, word_t *res){
  *cout = shamt > 0 && (n >> (shamt - 1));
  n >>= shamt;
  word_t mask = 1 << (31 - shamt);
  word_t trailing_ones = (n & mask) - 1;
  *res =  ~trailing_ones | n ;
}

void ror(byte_t shamt, word_t n, bool *cout, word_t *res){
  *res = (n >> shamt) | (n << (-shamt & 31));
  *cout = (*res >> 31) & 1;
}
