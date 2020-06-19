#include <stdlib.h>

#include "label_table.h"
#include "parser.h"
#include "tokenizer.h"

void free_assemble(machine_code *mcode, assembly_program *aspg,
                   symbol_table_t *labels, char **buffer, int file_lines) {
  free_mnemonic_table();
  free_machine_code(mcode);
  free_program(aspg);
  free_table(labels);
  free_buffer(buffer, file_lines);
}

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(
        stderr,
        "Assemble: Wrong argument!\nUsage:./assemble <path_to_assembly_file> "
        "<path_to_write_to>\n");
    return EXIT_FAILURE;
  }

  char **buffer = eCalloc(MAX_FILE_LINES, sizeof(char *));
  int file_lines = read_assembly_file(argv[1], buffer);
  buffer = eRealloc(buffer, file_lines * sizeof(char *));

  /* first pass*/
  assembly_program *aspg = tokenize(buffer, file_lines);
  symbol_table_t *labels = init_label_table(aspg);
  if (!init_mnemonic_table()) return EXIT_FAILURE;

  /* second pass*/
  machine_code *mcode = parse(aspg, labels);

  write_binary_file(argv[2], mcode->bin, mcode->length);
  free_assemble(mcode, aspg, labels, buffer, file_lines);
  return EXIT_SUCCESS;
}
