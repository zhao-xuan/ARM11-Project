#include "tokenizer.h"

static bool split_line(char *line, char **words) {
  char linecpy[strlen(line) + 1];
  strcpy(linecpy, line);

  char *token;
  if (strchr(linecpy, ':')) {
    token = strtok(linecpy, ":");
    words[0] = eMalloc(strlen(token) + 1);
    strncpy(words[0], token, strlen(token) + 1);
    token = strtok(NULL, " ");
    return true;
  }

  words[0] = NULL;
  token = strtok(linecpy, " ");
  words[1] = eMalloc(strlen(token) + 1);
  strncpy(words[1], token, strlen(token) + 1);
  token = strtok(NULL, "");
  words[2] = eMalloc(strlen(token) + 1);
  strncpy(words[2], token, strlen(token) + 1);
  return false;
}

assembly_program *tokenize(char **buffer, size_t max_line) {
  assembly_program *aspg = eCalloc(1, sizeof(assembly_program));
  aspg->lines = eCalloc(max_line, sizeof(assembly_line *));

  int instruction_count = 0;
  for (int i = 0; i < max_line && buffer[i]; i++) {
    assembly_line *asl = eMalloc(sizeof(assembly_line));
    char **sections = eCalloc(4, sizeof(char *));

    if (split_line(buffer[i], sections)) {
      asl->label = sections[0];
      split_line(buffer[++i], sections + 1);
      asl->opcode = sections[2];
      asl->operands = sections[3];
    } else {
      asl->label = sections[0];
      asl->opcode = sections[1];
      asl->operands = sections[2];
    }

    asl->location_counter = 4 * instruction_count;
    aspg->lines[instruction_count] = asl;
    free(sections);

    instruction_count++;  // Not the same as i as label skips line
  }
  aspg->total_lines = instruction_count;
  aspg->lines =
      eRealloc(aspg->lines, instruction_count * sizeof(assembly_line *));
  return aspg;
}

void free_program(assembly_program *aspg) {
  for (int i = 0; i < aspg->total_lines; i++) {
    if (aspg->lines[i]) {
      free(aspg->lines[i]->label);
      free(aspg->lines[i]->operands);
      free(aspg->lines[i]->opcode);
      free(aspg->lines[i]);
    }
  }
  free(aspg->lines);
  free(aspg);
}
