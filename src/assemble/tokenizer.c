#include "tokenizer.h"

static size_t split_line(char *line, char **words) {
  char linecpy[strlen(line) + 1];
  strcpy(linecpy, line);

  char *token = strtok(linecpy, " ");
  words[0] = malloc(strlen(token) + 1);

  int word_count;
  for (word_count = 0; token; word_count++) {
    words[word_count] = malloc(strlen(token) + 1);
    strcpy(words[word_count], token);
    token = strtok(NULL, " ");
  }

  return word_count;
}

assembly_program *tokenize(char **buffer, size_t max_line) {
  assembly_program *aspg = calloc(1, sizeof(assembly_program));
  aspg->lines = calloc(max_line, sizeof(assembly_line *));

  int instruction_count = 0;
  for (int i = 0; buffer[i]; i++) {
    assembly_line *asl = malloc(sizeof(assembly_line));
    char **sections = calloc(3, sizeof(char *));
    size_t num_blocks = split_line(buffer[i], sections);

    switch (num_blocks) {
      case 1:
        asl->label = sections[0];
        size_t num_blocks2 = split_line(buffer[i + 1], sections + 1);
        if (num_blocks2 != 2) fprintf(stderr, "labels should be followed by opcode and operands. ");
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
    asl->location_counter = 4 * instruction_count;
    aspg->lines[instruction_count] = asl;
    free(sections);

    instruction_count++;  // Not the same as i as label skips line
  }
  aspg->total_lines = instruction_count;
  aspg->lines = realloc(aspg->lines, instruction_count * sizeof(assembly_line *));
  return aspg;
}