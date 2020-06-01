#include "decode.h"
#include "execute.h"
#include "global.h"
#include "print.h"
#include "utils.h"

void pipeline();

int main(int argc, char **argv) {
  /* Example of how to integrate memory functions, not final*/
  init_state();
  word_t *buffer = NULL;
  size_t size;
  read_binary_file(argv[1], &buffer, &size);
  load_program(buffer, size);

  pipeline();

  print_state(stdout);
  free_state();
  return EXIT_SUCCESS;
}

void pipeline() {
  while (get_decoded().type != HALT) {
    instruction_t decoded_instr = get_decoded();
    if(decoded_instr.type != EMPTY){
      execute(decoded_instr);
    }

    word_t fetched_instr = get_fetched();
    if(fetched_instr != EMPTY_INSTR){
      instruction_t *decoded_addr = decode(fetched_instr);
      set_decoded(*decoded_addr);
    }

    word_t program_counter = get_and_incrementPC();
    set_fetched(get_word(program_counter));
  }
}