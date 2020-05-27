#include "logicunit.h"


void (*alu[7]) (word_t op1, word_t op2, bool *cout, word_t *res) = 
    { add, eor, sub, rsb, add, orr, mov }; 


void (*barrel_shifter[4]) (byte_t op1, word_t op2, bool *cout, word_t *res) =
    { lsl, lsr, asr, ror };

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
    bool overflow = op1 < 0 && (op2 > INT_MAX + op1);
    bool underflow = op1 > 0 && (op2 < INT_MAX + op1);
    *cout = overflow | underflow;
}


/*  REVERSE SUBTRACTION */
void rsb(word_t op1, word_t op2, bool *cout, word_t *res){
     sub(op2, op1, cout, res);
}

/*  ADDITION */
void add(word_t op1, word_t op2, bool *cout, word_t *res){
    *res = op1 + op2;
    bool overflow = op1 > 0 && (op2 > INT_MAX - op1);
    bool underflow = op1 < 0 && (op2 < INT_MAX - op1);
    *cout = overflow | underflow;
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
 *  @return: 32-bit unsigned integer
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
    *cout = shamt > 0 ? (n >> (shamt - 1)) & 1 : 0;
    n >>= shamt;
    word_t mask = 1 << (31 - shamt);
    word_t trailing_ones = (n & mask) - 1;
    *res =  ~trailing_ones | n ;
}

void ror(byte_t shamt, word_t n, bool *cout, word_t *res){
    *res = (n >> shamt) | (n << (-shamt & 31));
    *cout = (*res >> 31) & 1;
}
