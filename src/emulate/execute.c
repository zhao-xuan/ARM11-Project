#include "execute.h"

word_t fetch_instruction(state_t state) {
    word_t pre_instruction = state.fetched_instruction;
    state.fetched_instruction = state.memory[get_reg(PC)];
    set_reg(PC, get_reg(PC) + 4);
    return pre_instruction;
}

instruction_t decode_instruction(state_t state, word_t pre_instruction) {
    instruction_t *decoded_instruction = decode(pre_instruction);

}

int execute_instruction(state_t state);
int pipeline(state_t state);
