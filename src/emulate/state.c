#include "state.h"

static state_t *state;

void init_state() {
  state = (state_t *)calloc(1, sizeof(state_t));
  if (state == NULL) {
    fprintf(stderr, "Memory error!\n");
    exit(EXIT_FAILURE);
  }
  empty_pipeline();
}

void free_state() {
  free_instruction(state->decoded_instruction);
  free(state);
}

word_t get_and_incrementPC() {
  word_t value = get_reg(PC);
  set_reg(PC, value + 4U);
  return value;
}

word_t get_reg(int reg_no) {
  if (!out_of_bound_check(reg_no, REG_NUM)) {
    return state->registers[reg_no];
  }
  return 0;
}

void set_reg(int reg_no, word_t value) {
  if (!out_of_bound_check(reg_no, REG_NUM)) {
    state->registers[reg_no] = value;
  }
}

bool get_flag(flag_t flag) {
  word_t curr = get_reg(CPSR);
  return (curr >> flag) & 1U;
}

void set_flag_to(flag_t flag, bool x) {
  word_t reg_val = get_reg(CPSR);
  reg_val ^= (-x ^ reg_val) & (1U << flag);
  set_reg(CPSR, reg_val);
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
  if (!out_of_bound_check(addr, MEM_ADDR) &&
      !out_of_bound_check(addr, MEM_ADDR + 3)) {
    word_t word;
    for (int i = 0; i < 4; i++) {
      ((byte_t *)&word)[i] = state->memory[addr + i];
    }
    return word;
  }
  return 0;
}

void set_word(address_t addr, word_t word) {
  if (!out_of_bound_check(addr, MEM_ADDR) &&
      !out_of_bound_check(addr, MEM_ADDR + 3)) {
    for (int i = 0; i < 4; i++) {
      byte_t current_byte = ((byte_t *)&word)[i];
      state->memory[addr + i] = current_byte;
    }
  }
}

void load_program(word_t *buffer, size_t size) {
  for (int i = 0; i < size; i++) {
    set_word(i << 2, buffer[i]);
  }
}

word_t get_fetched() { return state->fetched_instruction; }

void set_fetched(word_t fetched_instruction) {
  state->fetched_instruction = fetched_instruction;
}

instruction_t *get_decoded() { return state->decoded_instruction; }

void set_decoded(instruction_t *decoded_instruction) {
  free_instruction(state->decoded_instruction);
  state->decoded_instruction = decoded_instruction;
}

void empty_pipeline() {
  instruction_t *empty_instruction = malloc(sizeof(instruction_t));
  empty_instruction->type = EMPTY;
  set_fetched(EMPTY_INSTR);
  set_decoded(empty_instruction);
}

void free_instruction(instruction_t *prev) {
  if (prev != NULL) {
    switch (prev->type) {
      case DATA_PROCESSING: {
        data_processing_t *data_process = prev->instructions.data_processing;
        if (data_process->imm_const) {
          free(data_process->operand2.imm_value);
        } else {
          free(data_process->operand2.reg_value);
        }
        free(data_process);
        break;
      }
      case MULTIPLY: {
        free(prev->instructions.multiply);
        break;
      }
      case DATA_TRANSFER: {
        free(prev->instructions.data_transfer);
        break;
      }
      case BRANCH: {
        free(prev->instructions.branch);
        break;
      }
      default:
        break;
    }
    free(prev);
  }
}