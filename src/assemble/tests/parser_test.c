/*
 * @brief: deleting any leading spaces, '[' and ']' in a string. This function would only deal with white spaces at the beginning of the string or '[' and ']'
 * @param: char *str: the string to be processed
 * @return: str without leading spaces
 */
static char *trim_field(char *str) {
  int i = 0;
  while (*(str + i) == ' ' || *(str + i) == '[') {
    i++;
  }
  char *trimmed = malloc((strlen(str) - i) * sizeof(char));
  strcpy(trimmed, str + i);
  if (*(str + strlen(str) - 1) == ']') {
    trimmed = realloc(trimmed, strlen(trimmed) - 1);
  }
  free(str);
  return trimmed;
}

static char **operand_processor(char *operand, int field_count) {
  char **tokens = calloc(field_count, sizeof(char *));
  int i = 0;
  char *literal = strtok(operand, ",");
  while (literal != NULL || i < field_count) {
    tokens[i] = malloc(strlen(literal) * sizeof(char));
    strcpy(token[i], literal);
    i++;
    literal = strtok(NULL, ",");
  }
  tokens = realloc(tokens, i * sizeof(char *));

  return tokens;
}

static void free_tokens(char **tokens) {
  int i = 0;
  while (tokens[i] != NULL) {
    free(tokens[i]);
    i++;
  }

  free(tokens);
}

int main(void) {
  /* Testing trim_field() ... */
  char *name = "Testing if the helper function trim_field works properly";
  teststring(trim_field(" r1"), "r1", name);
  teststring(trim_field("  r3"), "r3", name);
  teststring(trim_field("[r2]"), "r2", name);
  teststring(trim_field("[r15"), "r15", name);

  /* Testing operand_processor */
  name = "Testing if the operand_processor() works properly";
  char **tokens1 = operand_processor("r1,r2,r3", 3);
  teststring(tokens[0], "r1", name);
  teststring(tokens[1], "r2", name);
  teststring(tokens[2], "r3", name);
  free_tokens(tokens1);
  char **tokens2 = operand_processor("r1,r2,lsl #2", 3);
  teststring(tokens[0], "r1", name);
  teststring(tokens[1], "r2", name);
  teststring(tokens[2], "lsl #2", name);
  free_tokens(tokens2);
  char **tokens3 = operand_processor("r0,=0x555555", 3);
  teststring(tokens[0], "r0", name);
  teststring(tokens[1], "=0x555555", name);
  free_tokens(tokens3);
  char **token4 = operand_processor("[r4],-r3,lsl #2", 3);
  teststring(tokens[0], "r1", name);
  teststring(tokens[1], "r2", name);
  teststring(tokens[2], "r3", name);
  free_tokens(tokens4);
}
