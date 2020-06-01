/*
 * Defines the state of the machine
 * The functions defined this file automatically converts to and from little endian. 
 * If conversion fails, please fix the functions here (or create an issue) instead of fixing it later in the code. 
 */

#ifndef STATE_H
#define STATE_H

#include "global.h"
#include "instructions.h"
#include "print.h"
#include "utils.h"

/* Structure for Memory, Registers and the Pipeline */
typedef struct {
  /* Memory*/
  byte_t memory[MEM_ADDR];
  /* Registers*/
  word_t registers[REG_NUM];
  /* Instruction that has already been fetched in the pipeline */
  word_t fetched_instruction;
  /* Instruction that has already been decoded in the pipeline */
  instruction_t decoded_instruction;
} state_t;

/*
 * Callocs space in heap for memory and registers
 */
void init_state();


/*
 * Free the heap storing the structure 
 */
void free_state();

/*
 * Return the current value of the Program counter, then increments it. 
 */
word_t get_and_incrementPC();

/*
 * @param reg_no : number of the required register. 
 * @returns: value of the register. (In big endian)
 */
word_t get_reg(int reg_no);

/*
 * @param: reg_no : number of the required register.
 * @param: value  : Value to be set. (In big endian)
 * @brief: sets the value of the register to value specified.
 */
void set_reg(int reg_no, word_t value);

/*
 * @param: flag  : C_FLAG, N_FLAG, V_FLAG, Z_FLAG
 * @param: x     : Boolean to set the flag to
 */
void set_flag_to(flag_t flag, bool x);

/*
 * @param: flag  : C_FLAG, N_FLAG, V_FLAG, Z_FLAG
 * @return: whether the flag in question is set or not. 
 */
bool get_flag(flag_t flag);

/*
 * @param: flag  : C_FLAG, N_FLAG, V_FLAG, Z_FLAG
 * @brief: sets the flag in question to 1.  
 */
void set_flag(flag_t flag);


/*
 * @param: flag  : C_FLAG, N_FLAG, V_FLAG, Z_FLAG
 * @brief: sets the flag in question to 0.  
 */
void clear_flag(flag_t flag);

/*
 * @param: addr : address of the required memory position.
 * @returns: gets 32 bits from the memory starting from the specified address. (In big endian)
 */
word_t get_word(address_t addr);

/*
 * @param: addr   : address of the required memory position.
 * @param: value  : Value to be set.(In big endian)
 * @brief: sets 32 bits in the memory starting from the specified address. 
 */
void set_word(address_t addr, word_t instruction);

/*
 * @param: addr : address of the required memory position
 * @brief: gets 8 bits from the memory starting from the specified address. (In big endian)
 */
byte_t get_memory(address_t addr);

/*
 * @param: addr   : address of the required memory position.
 * @param: value  : Value to be set. (In big endian)
 * @brief: sets 8 bits in the memory starting from the specified address.
 */
void set_memory(address_t addr, byte_t value);

/*
 * @param: *buffer   : Buffer containing program to load.
 * @param: size      : Number of words of the buffer specified.
 * @brief: Loads program in buffer into the memory.
 */
void load_program(word_t *buffer, size_t size);

/*
 * @param: fetched_instruction   : the value to set fetched_instruction
 * @brief: sets fetched_instruction to the param specified. 
 */
void set_fetched(word_t fetched_instruction);


/*
 * @returns: the value of fetched_instruction in the current state. 
 */
word_t get_fetched();

/*
 * @param: decoded_instruction   : the value to set decoded_instruction
 * @brief: sets decoded_instruction to the param specified. 
 */
void set_decoded(instruction_t decoded_instruction);



/*
 * @returns: the value of decoded_instruction in the current state. 
 */
instruction_t get_decoded();

/*
 * sets fetched and decoded both to empty. 
 */
void empty_pipeline();


#endif
