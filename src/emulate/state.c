/*
 * Implements the data structure and interfaces for the state of the machine.
 * The data structure is fully encapsulated, and can only be modified through
 * public interfaces.
 */

#include "state.h"

#include <stdio.h>

#include "exceptions.h"
#include "utils.h"

/* Structure for Memory, Registers and the Pipeline */
typedef struct {
  /* Memory */
  byte_t memory[MEM_ADDR];
  /* Registers */
  word_t registers[REG_NUM];
  /* Instruction that has already been fetched in the pipeline */
  word_t fetched_instruction;
  /* Instruction that has already been decoded in the pipeline */
  instruction_t *decoded_instruction;
} state_t;

/*
 * A static field representing the ONLY state of the machine.
 * A machine can NOT have more than 1 states.
 */
static state_t *state;

static bool isGPIO(address_t addr);
static bool isONOFF(address_t addr);

void init_state() {
  state = (state_t *)eCalloc(1, sizeof(state_t));
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

word_t get_reg(byte_t reg_no) {
  if (reg_no < REG_NUM) {
    return state->registers[reg_no];
  }
  exceptions(REGISTER_INDEX_OUT_OF_BOUND, reg_no);
  return 0;
}

void set_reg(byte_t reg_no, word_t value) {
  if (reg_no < REG_NUM) {
    state->registers[reg_no] = value;
    return;
  }
  exceptions(REGISTER_INDEX_OUT_OF_BOUND, reg_no);
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
  if (addr + 3 < MEM_ADDR) {
    word_t word;
    for (int i = 0; i < 4; i++) {
      ((byte_t *)&word)[i] = state->memory[addr + i];
    }
    return word;
  }
  if (isGPIO(addr)) return addr;
  exceptions(MEMORY_INDEX_OUT_OF_BOUND, addr);
  return 0;
}

void set_word(address_t addr, word_t word) {
  if (addr + 3 < MEM_ADDR) {
    for (int i = 0; i < 4; i++) {
      byte_t current_byte = ((byte_t *)&word)[i];
      state->memory[addr + i] = current_byte;
    }
    return;
  }
  if (isGPIO(addr) || isONOFF(addr)) return;
  exceptions(MEMORY_INDEX_OUT_OF_BOUND, addr);
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
  instruction_t *empty_instruction = eMalloc(sizeof(instruction_t));
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
        if (prev->instructions.data_transfer->imm_offset) {
          free(prev->instructions.data_transfer->offset.reg_value);
        }
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


static bool isGPIO(address_t addr) {
  if (addr == 0x20200000) {
    printf("One GPIO pin from 0 to 9 has been accessed\n");
    return true;
  }
  if (addr == 0x20200004) {
    printf("One GPIO pin from 10 to 19 has been accessed\n");
    return true;
  }
  if (addr == 0x20200008) {
    printf("One GPIO pin from 20 to 29 has been accessed\n");
    return true;
  }
  return false;
}

static bool isONOFF(address_t addr) {
  if (addr == 0x20200028) {
    printf("PIN OFF\n");
    return true;
  }
  if (addr == 0x2020001c) {
    printf("PIN ON\n");
    return true;
  }
  return false;
}