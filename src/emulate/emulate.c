#include "utils.h"
#include "print.h"
#include "decode.h"
#include "execute.h"
#include "global.h"

int main(int argc, char **argv) {

  /* Example of how to integrate memory functions, not final*/
  init_state();
  word_t *buffer = NULL;
  size_t size; 
  read_binary_file(argv[1], &buffer, &size);
  load_program(buffer, size);
  
  set_flag(N_FLAG);

  print_state(stdout);
  free_state();
  return EXIT_SUCCESS;
}
