#include "tokenizer.h"

assembly_program tokenize(char **buffer, size_t max_line) {
  assembly_program aspg = calloc(1, sizeof(assembly_program));
  address_t lc = 0U;

  for (int i = 0; buffer[i]; i++) {
    lc += 4;  // Not the same as 4 * i as label skips line
    assembly_line asl = malloc(sizeof(assembly_line));
    char **sections = calloc(3, sizeof(char *));
    size_t num_blocks = split_line(buffer[i], sections);

    switch (num_blocks) {
      case 1:
        asl->label = sections[0];
        size_t num_blocks2 = split_line(buffer[i + 1], sections + 1);
        if (num_blocks2 != 2) fprintf(stderr, "shouldnt happen. ");
        asl->opcode = sections[1];
        asl->operands = sections[2];
        i++;  // skip next file line
        break;
      case 2:
        asl->label = NULL;
        asl->opcode = sections[0];
        asl->operands = sections[1];
        break;
      case 3:
        asl->label = sections[0];
        asl->opcode = sections[1];
        asl->operands = sections[2];
        break;
    }
    asl->location_counter = lc;
    aspg->lines[i] = asl;
    free(sections);
  }
  return aspg;
}

size_t split_line(char *line, char **words) {
  words[0] = malloc(MAX_LINE_LENGTH);
  char linecpy[MAX_LINE_LENGTH];
  strcpy(linecpy, line);
  char *token = strtok(linecpy, " ");
  int word_count;
  for (word_count = 0; token; word_count++) {
    words[word_count] = malloc(MAX_LINE_LENGTH);
    strcpy(words[word_count], token);
    token = strtok(NULL, " ");
  }

  return word_count;
}
