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

  sprintf(out, "{.type: %-15s, .bin: 0x%08x}", type, ptr->bin); 

}


void testmnemonic(mnemonic_p expected, mnemonic_p got, char *testname) {
  bool match = expected->bin == got->bin
            && expected->type == got->type;
  switch_color(match);
  char exp[50], out[50];
  print_mnemonic(expected, exp);
  print_mnemonic(got, out);
  printf("T %s: ----------------- %s \n  expected=%s \n       got=%s\n", 
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
  

  /* Multiply */

  /* mul 11100000_00000000_00000000_10010000*/
  testname = "Mul should return correct binary";
  SET_EXP(MULTIPLY, 3758096528);
  TEST_MNEMONIC("mul");

  /* mla 11100000_00100000_00000000_10010000 */
  testname = "Mla should return correct binary";
  SET_EXP(MULTIPLY, 3760193680);
  TEST_MNEMONIC("mla");


  /* Single Data Transfer */

  /* ldr 11100100_00010000_00000000_00000000 */
  testname = "Ldr should return correct binary";
  SET_EXP(DATA_TRANSFER, 3826253824);
  TEST_MNEMONIC("ldr");

  /* str 11100100_00000000_00000000_00000000 */
  testname = "Str should return correct binary";
  SET_EXP(DATA_TRANSFER, 3825205248);
  TEST_MNEMONIC("str");


  /* Branch instruction */
   
  /* beq 00001010_00000000_00000000_00000000 */
  testname = "Beq should return correct binary";
  SET_EXP(BRANCH, 167772160);
  TEST_MNEMONIC("beq");

  /* bne 00011010_00000000_00000000_00000000 */
  testname = "Bne should return correct binary";
  SET_EXP(BRANCH, 436207616);
  TEST_MNEMONIC("bne");

  /* bge 10101010_00000000_00000000_00000000 */
  testname = "Bge should return correct binary";
  SET_EXP(BRANCH, 2852126720);
  TEST_MNEMONIC("bge");
  
  /* blt 10111010_00000000_00000000_00000000 */
  testname = "Blt should return correct binary";
  SET_EXP(BRANCH, 3120562176);
  TEST_MNEMONIC("blt");

  /* bgt 11001010_00000000_00000000_00000000 */
  testname = "Bgt should return correct binary";
  SET_EXP(BRANCH, 3388997632);
  TEST_MNEMONIC("bgt");

  /* ble 11011010_00000000_00000000_00000000 */
  testname = "ble should return correct binary";
  SET_EXP(BRANCH, 3657433088);
  TEST_MNEMONIC("ble");

  /* bal 11101010_00000000_00000000_00000000 */
  testname = "Bal should return correct binary";
  SET_EXP(BRANCH, 3925868544);
  TEST_MNEMONIC("bal");

  /* b   11101010_00000000_00000000_00000000*/
  testname = "B (Branch always without suffix) returns correct binary";
  TEST_MNEMONIC("b");

  /* Special instruction */
  /* Halt */
  testname = "Andeq should return correct binary";
  SET_EXP(HALT, 0);
  TEST_MNEMONIC("andeq");
  
  /* Unknown instructions */
  testname = "Unknown instruction should return NULL";
  testbool(get_mnemonic_data("nonsense") == NULL, testname);

  /* Free table */
  free_mnemonic_table();
  testname = "Table can be freed correctly";
  testbool(true, testname);
  

  return 0;
}

