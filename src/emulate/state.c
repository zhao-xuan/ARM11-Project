#include "state.h"

static state_t *state;

void init_state() {
  state = (state_t *)calloc(1, sizeof(state_t));
  if (state == NULL) {
    fprintf(stderr, "Memory error!\n");
    exit(EXIT_FAILURE);
  }
}

void return_state(FILE* fp) {
  print_state(fp);
  free(state);
}

word_t get_reg(int reg_no) {
  out_of_bound_check(reg_no, REG_NUM);
  word_t value = state->registers[reg_no];
  return value;
}

void set_reg(word_t reg_no, word_t value) {
  out_of_bound_check(reg_no, REG_NUM);
  state->registers[reg_no] = value;
}

bool get_flag(flag_t flag) {
  word_t curr = get_reg(CPSR);
  return (curr >> flag) & 1U;
}

void set_flag(flag_t flag) {
  word_t reg_val = get_reg(CPSR);
  reg_val |= 1U << flag;
  set_reg(CPSR, reg_val);
}

void clear_flag(flag_t flag) {
  word_t reg_val = get_reg(CPSR);
  reg_val &= ~(1U << flag);
  set_reg(CPSR, reg_val);
}

word_t get_word(address_t addr) {
  word_t word;
  for (int i = 0; i < 4; i++) {
    ((byte_t *)&word)[i] = get_memory(addr + i);
  }
  /* swaps little endian to big endian */
  swap_endian(&word, 1);
  return word;
}

void set_word(address_t addr, word_t word) {
  
  /* swaps big endian to little endian */
  swap_endian(&word, 1);
  for (int i = 0; i < 4; i++) {
    byte_t current_byte = ((byte_t *)&word)[i];
    set_memory(addr + i, current_byte);
  }
}

byte_t get_memory(address_t addr) {
  out_of_bound_check(addr, MEM_ADDR);
  return state->memory[addr];
}

void set_memory(address_t addr, byte_t value) {
  out_of_bound_check(addr, MEM_ADDR);
  state->memory[addr] = value;
}

void load_program(word_t *buffer, size_t size) {
  for (int i = 0; i < size; i++) {
    set_word(i << 2, buffer[i]);
  }
}
