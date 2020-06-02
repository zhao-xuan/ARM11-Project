/*
 * Defines the interfaces for modifying the state of the machine
 * It is required to call the constructor init_state() before modifying the state
 * and call the destructor free_state() before exitting the program.
 */

#ifndef STATE_H
#define STATE_H

#include <stdlib.h>
#include "instructions.h"

/*
 * @brief: Constructor for the state.
 * Allocate memory spaces on the heap for the state.
 */
void init_state();

/*
 * @brief: Destructor for the state.
 * Free the memory spaces used by the state structure. 
 */
void free_state();

/*
 * Get the current value of PC and increments it by 4.
 * @returns: the current value of Program Counter.
 */
word_t get_and_incrementPC();

/*
 * Get the value stored in the register.
 * @param reg_no : number of the required register. 
 * @returns: value stored in the register.
 */
word_t get_reg(int reg_no);

/*
 * Sets the value stored in the register.
 * @param reg_no : number of the required register.
 * @param value  : value to be set to.
 */
void set_reg(int reg_no, word_t value);

/*
 * Sets or clears the flag (depends on input).
 * @param flag : flag to be set or cleared.
 * @param x    : value of the flag to be set to.
 */
void set_flag_to(flag_t flag, bool x);

/*
 * Gets the value of a flag in CPSR register.
 * @param flag : a flag in CPSR register.
 * @returns : whether the flag is set or not. 
 */
bool get_flag(flag_t flag);

/*
 * Sets a flag in CPSR register.
 * @param flag : flag to be set.
 */
void set_flag(flag_t flag);

/*
 * Clear a flag in CPSR register.
 * @param flag : flag to be cleared.
 */
void clear_flag(flag_t flag);

/*
 * Fetch a word from the memory
 * @param addr : address of the required memory position.
 * @returns: gets 32 bits from the memory starting from the specified address.
 */
word_t get_word(address_t addr);

/*
 * Sets the memory content by a word.
 * @param addr  : address of the required memory position.
 * @param value : Value to be set.
 */
void set_word(address_t addr, word_t word);

/*
 * Loads the program in the buffer into the memory.
 * @param *buffer : pointer to the buffer containing the program to load.
 * @param size    : the number of words of the buffer specified.
 */
void load_program(word_t *buffer, size_t size);

/*
 * Store a fetched binary instruction to the pipeline.
 * @param fetched_instruction : the binary instruction fetched.
 */
void set_fetched(word_t fetched_instruction);

/*
 * Gets the fetched binary instruction in the pipeline.
 * @returns: the value of binary instruction in the current pipeline. 
 */
word_t get_fetched();

/*
 * Stores a decoded instruction to the pipeline.
 * @param decoded_instruction : the instruction decoded.
 */
void set_decoded(instruction_t *decoded_instruction);

/*
 * Gets the decoded instruction in the pipeline.
 * @returns: a pointer to the decoded instruction in the current pipeline. 
 */
instruction_t *get_decoded();

/*
 * Empties the pipeline. 
 */
void empty_pipeline();

/*
 * Frees the instruction pointed to by prev.
 * @param *prev: a pointer to the decoded instruction that needs to be freed.
 */
void free_instruction(instruction_t *prev);

#endif
