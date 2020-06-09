#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "testutils.h"
#include "mnemonic_table.h"

#define TEST_MNEMONIC(mnemonic)\
  testmnemonic(expected, get_mnemonic_data(mnemonic), testname) 

#define SET_EXP(instr, value)\
  expected->type = instr; expected->bin = value

void print_mnemonic(mnemonic_p ptr, char *out) {
  char *type;
  switch (ptr->type) {
    case(DATA_PROCESSING):
      type = "Data Processing";
      break;
    case(DATA_TRANSFER):
      type = "Data Transfer";
      break;
    case(MULTIPLY):
      type = "Multiply";
      break;
    case(BRANCH):
      type = "Branch";
      break;
    case(HALT):
      type = "Halt";
      break;
    default:
      type = "Unknown Type";
  }

  sprintf(out, "{.type: %15s, .bin: 0x%08x}", type, ptr->bin); 

}


void testmnemonic(mnemonic_p expected, mnemonic_p got, char *testname) {
  bool match = expected->bin == got->bin
            && expected->type == got->type;
  switch_color(match);
  char exp[50], out[50];
  print_mnemonic(expected, exp);
  print_mnemonic(got, out);
  printf("T %s:  %s \n  expected=%s \n       got=%s)\n", 
    testname, match ? "OK" : "FAIL", exp, out);
} 


int main() {
  char *testname;
  mnemonic_p expected = malloc(sizeof(mnemonic_t));

  /* Querying uninitialised table should return NULL */
  testname = "Querying uninitialised table should return NULL";
  testbool(!get_mnemonic_data("nothing"), testname);
  
  /* Initialise table */
  testname = "Table should have been initialised";
  testbool(init_mnemonic_table(), testname);
  
  /* Querying empty table */
  testname = "Querying empty table should return NULL";
  testbool(!get_mnemonic_data("nothing"), testname);

  /* Below are the tests for each of the instruction */
  
  /* DATA PROCESSING */

  /* and  11100000_00000000_0000000_00000000 */
  testname = "And should return correct binary";
  SET_EXP(DATA_PROCESSING, 3758096384);
  TEST_MNEMONIC("and");

  /* eor 11100000_00100000_00000000_00000000 */
  testname = "Eor should return correct binary";
  SET_EXP(DATA_PROCESSING, 3760193536);
  TEST_MNEMONIC("eor");
 
  /* sub 11100000_01000000_00000000_00000000 */
  testname = "Sub should return correct binary";
  SET_EXP(DATA_PROCESSING, 3762290688);
  TEST_MNEMONIC("sub");

  /* rsb 11100000_01100000_00000000_00000000 */
  testname = "Rsb should return correct binary";
  SET_EXP(DATA_PROCESSING, 3764387840);
  TEST_MNEMONIC("rsb");
  
  /* add 11100000_10000000_00000000_00000000 */
  testname = "Add should return correct binary";
  SET_EXP(DATA_PROCESSING, 3766484992);
  TEST_MNEMONIC("add");

  /* orr 11100001_10000000_00000000_00000000 */
  testname = "Orr should return correct binary";
  SET_EXP(DATA_PROCESSING, 3783262208);
  TEST_MNEMONIC("orr");
  
  /* mov 11100001_10100000_00000000_00000000 */
  testname = "Mov should return correct binary";
  SET_EXP(DATA_PROCESSING, 3785359360);
  TEST_MNEMONIC("mov");
 
  /* tst 11100001_00010000_00000000_00000000 */
  testname = "Tst should return correct binary";
  SET_EXP(DATA_PROCESSING, 3775922176);
  TEST_MNEMONIC("tst");
 
  /* teq 11100001_00110000_00000000_00000000 */
  testname = "Teq should return correct binary";
  SET_EXP(DATA_PROCESSING, 3778019328);
  TEST_MNEMONIC("teq");

  /* cmp 11100001_01010000_00000000_00000000 */
  testname = "Cmp should return correct binary";
  SET_EXP(DATA_PROCESSING, 3780116480);
  TEST_MNEMONIC("cmp");
  return 0;
}

