#include <stdlib.h>

#include "label_table.h"
#include "table.h"
#include "tokenizer.h"

int main(int argc, char **argv) {

  char **buffer = calloc(MAX_FILE_LINES, sizeof(char *));
  int file_lines = read_assembly_file(argv[1], buffer);
  assembly_program *aspg = tokenize(buffer, file_lines);
  symbol_table_t *labels = init_label_table(aspg);
  printf("%d\n", get_label_address(labels, "wait"));

  free(buffer);
  return EXIT_SUCCESS;
}
