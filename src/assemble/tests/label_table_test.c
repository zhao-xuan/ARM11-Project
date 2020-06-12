#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "testutils.h"
#include "label_table.h"
#include "tokenizer.h"

int main(){
  /* Taken from tokenizer test */
  char *name;

 
  /*-----------Test 1----------*/

  /* loop01 */
  assembly_program *program1;
  char **buffer1 = eCalloc(5, sizeof(char *));
  buffer1[0] = "mov r2,#0x3F0000";
  buffer1[1] = "wait:";
  buffer1[2] = "sub r2,r2,#1";
  buffer1[3] = "cmp r2,#0xFF";
  buffer1[4] = "bne wait";
  name = "loop01: tokenized correctly";
  program1 = tokenize(buffer1, 5);
  teststring(program1->lines[0]->operands, "r2,#0x3F0000", name);
  teststring(program1->lines[1]->label, "wait", name);
  teststring(program1->lines[1]->opcode, "sub", name);
  testint(program1->lines[2]->location_counter, 8, name);
  teststring(program1->lines[3]->operands, "wait", name);
  testint(program1->total_lines, 4, name);
  printf("\n");

  /* Symbol table */
  symbol_table_t *table1 = init_label_table(program1);
  name = "Querying 'wait' should return correct address";
  testint(get_label_address(table1, "wait"), 4, name);

  free(buffer1);
  free_table(table1);
  free_program(program1);

  /*---------Test 2----------*/

  /* opt_ldr12 (modified) */
  assembly_program *program2;
  char **buffer2 = eCalloc(5, sizeof(char *));
  buffer2[0] = "mov r1,#4128768";
  buffer2[1] = "ldr r2,[r1]";
  buffer2[2] = "ldr r3,[r1,r2,lsr #2]";
  buffer2[3] = "str r4, [r1, r2, lsr  r4]";
  buffer2[4] = "ldr r1, =0x42";
  name = "opt_ldr12 (modified): tokenized correctly";
  program2 = tokenize(buffer2, 5);
  teststring(program2->lines[1]->operands, "r2,[r1]", name);
  teststring(program2->lines[2]->operands, "r3,[r1,r2,lsr #2]", name);
  teststring(program2->lines[3]->opcode, "str", name);
  teststring(program2->lines[3]->operands, "r4, [r1, r2, lsr  r4]", name);
  testpointer(program2->lines[4]->label, NULL, name);
  printf("\n");
  

  /* Symbol table */
  symbol_table_t *table2 = init_label_table(program2);
  name = "Assembler with no label should return -1";
  testint(get_label_address(table2, "none"), -1, name);

  free(buffer2);
  free_table(table2);
  free_program(program2);
  

  /*------------Test 3---------------*/

  /* opt_add & tst04 */
  assembly_program *program3;
  char **buffer3 = eCalloc(5, sizeof(char *));
  buffer3[0] = "mov r1,#1";
  buffer3[1] = "b foo";
  buffer3[2] = "foo:";
  buffer3[3] = "mov r4 #4";
  buffer3[4] = "sub r5,r4, r3, lsr r2";
  name = "opt_add & tst04: tokenized correctly";
  program3 = tokenize(buffer3, 5);
  teststring(program3->lines[1]->opcode, "b", name);
  teststring(program3->lines[2]->label, "foo", name);
  teststring(program3->lines[2]->opcode, "mov", name);
  teststring(program3->lines[3]->operands, "r5,r4, r3, lsr r2", name);
  testint(program3->total_lines, 4, name);
  printf("\n");
  
  /* Symbol table */
  symbol_table_t *table3 = init_label_table(program3);
  name = "Backward referencing label can be added correctly";
  
  free(buffer3);
  free_table(table3);
  free_program(program3);


  return 0;
}
