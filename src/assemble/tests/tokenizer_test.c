/*
 * Run ./tokenizer_test
 * Run valgrind to detect memory leaks.
 */

#include "testutils.h"
#include "tokenizer.h"

int main(void) {

  /* add03 */
  char **buffer = eCalloc(2, sizeof(char *));
  buffer[0] = "mov r1,#1";
  buffer[1] = "add r1,r1,r1";
  char *name = "add03: tokenized correctly";
  assembly_program *program = tokenize(buffer, 2);
  testpointer(program->lines[0]->label, NULL, name);
  teststring(program->lines[0]->opcode, "mov", name);
  teststring(program->lines[0]->operands, "r1,#1", name);
  testpointer(program->lines[1]->label, NULL, name);
  teststring(program->lines[1]->opcode, "add", name);
  teststring(program->lines[1]->operands, "r1,r1,r1", name);
  printf("\n");
  free(buffer);
  free_program(program);

  /* loop01 */
  buffer = eCalloc(5, sizeof(char *));
  buffer[0] = "mov r2,#0x3F0000";
  buffer[1] = "wait:";
  buffer[2] = "sub r2,r2,#1";
  buffer[3] = "cmp r2,#0xFF";
  buffer[4] = "bne wait";
  name = "loop01: tokenized correctly";
  program = tokenize(buffer, 5);
  teststring(program->lines[0]->operands, "r2,#0x3F0000", name);
  teststring(program->lines[1]->label, "wait", name);
  teststring(program->lines[1]->opcode, "sub", name);
  testint(program->lines[2]->location_counter, 8, name);
  teststring(program->lines[3]->operands, "wait", name);
  testint(program->total_lines, 4, name);
  printf("\n");
  free(buffer);
  free_program(program);

  /* opt_ldr12 (modified) */
  buffer = eCalloc(5, sizeof(char *));
  buffer[0] = "mov r1,#4128768";
  buffer[1] = "ldr r2,[r1]";
  buffer[2] = "ldr r3,[r1,r2,lsr #2]";
  buffer[3] = "str r4, [r1, r2, lsr  r4]";
  buffer[4] = "ldr r1, =0x42";
  name = "opt_ldr12 (modified): tokenized correctly";
  program = tokenize(buffer, 5);
  teststring(program->lines[1]->operands, "r2,[r1]", name);
  teststring(program->lines[2]->operands, "r3,[r1,r2,lsr #2]", name);
  teststring(program->lines[3]->opcode, "str", name);
  teststring(program->lines[3]->operands, "r4, [r1, r2, lsr  r4]", name);
  testpointer(program->lines[4]->label, NULL, name);
  printf("\n");
  free(buffer);
  free_program(program);

  /* opt_add & tst04 */
  buffer = eCalloc(5, sizeof(char *));
  buffer[0] = "mov r1,#1";
  buffer[1] = "b foo";
  buffer[2] = "foo:";
  buffer[3] = "mov r4 #4";
  buffer[4] = "sub r5,r4, r3, lsr r2";
  name = "opt_add & tst04: tokenized correctly";
  program = tokenize(buffer, 5);
  teststring(program->lines[1]->opcode, "b", name);
  teststring(program->lines[2]->label, "foo", name);
  teststring(program->lines[2]->opcode, "mov", name);
  teststring(program->lines[3]->operands, "r5,r4, r3, lsr r2", name);
  testint(program->total_lines, 4, name);
  printf("\n");
  free(buffer);
  free_program(program);

  return 0;
}
