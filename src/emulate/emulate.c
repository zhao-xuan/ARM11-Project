/*
 * Entry point of the emulate module.
 * Initializes the emulated machine and implements the pipeline.
 */

#include <stdio.h>

#include "decode.h"
#include "execute.h"
#include "print.h"
#include "utils.h"

void pipeline();

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(
        stderr,
        "Emulate: Wrong argument!\nUsage: ./emulate <path_to_binary_file>\n");
    return EXIT_FAILURE;
  }

  word_t *buffer  = (word_t *) malloc(MEM_ADDR);
  size_t size;
  read_binary_file(argv[1], buffer, &size);
  init_state();
  load_program(buffer, size);

  pipeline();

  free(buffer);
  print_state(stdout);
  free_state();
  return EXIT_SUCCESS;
}

/*
 * Implementation of the three-staged pipeline of an ARM11 processor.
 */

void pipeline() {
  while (get_decoded()->type != HALT) {
    instruction_t *decoded_instr = get_decoded();
    if (decoded_instr->type != EMPTY) {
      execute(decoded_instr);
    }

    word_t fetched_instr = get_fetched();
    if (fetched_instr != EMPTY_INSTR) {
      instruction_t *decoded_addr = decode(fetched_instr);
      set_decoded(decoded_addr);
    }

    word_t program_counter = get_and_incrementPC();
    set_fetched(get_word(program_counter));
  }
}
