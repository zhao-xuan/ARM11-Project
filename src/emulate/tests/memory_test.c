/*
 * Unit tests for state.c
 * Run ./memory_test
 */

#include <stdio.h>

#include "global.h"
#include "print.h"
#include "state.h"
#include "testutils.h"
#include "utils.h"

static void test_reg_operations() {
  /*
   * Testing get_reg(word_t reg_no) and set_reg(word_t reg_no, word_t value):
   * Test if the register can be set and accessed properly, and if they are
   * initialized to 0 and in big endian format
   */
  char *name;

  for (word_t i = 0; i < REG_NUM; i++) {
    /* Testing register initialization */
    name = "Testing if registers initialized to 0";
    testword(get_reg(i), 0, name);
    set_reg(i, i);
    /* Testing register set */
    name = "Testing if registers can be set properly";
    testword(get_reg(i), i, name);
  }
}

static void test_flag_operations() {
  /*
   * Testing get_flag(flag_t flag), set_flag(flag_t flag), and clear_flag(flag_t
   * flag): Test if the flags can be set and accessed properly
   */
  char *name;

  /* Testing flag initialization */
  name = "Testing if flags are initialized to 0";
  testbool(!(get_flag(C_FLAG) || get_flag(N_FLAG) || get_flag(V_FLAG) ||
             get_flag(Z_FLAG)),
           name);
  set_flag_to(C_FLAG, 1);
  set_flag_to(N_FLAG, 1);
  set_flag_to(V_FLAG, 1);
  set_flag_to(Z_FLAG, 1);

  /* Test flag set */
  name = "Testing if flags can be set properly";
  testbool(get_flag(C_FLAG) && get_flag(N_FLAG) && get_flag(V_FLAG) &&
               get_flag(Z_FLAG),
           name);
  set_flag_to(C_FLAG, 0);
  set_flag_to(N_FLAG, 0);
  set_flag_to(V_FLAG, 0);
  set_flag_to(Z_FLAG, 0);

  /* Test flag clear */
  name = "Testing if flags can be cleared properly";
  testbool(!(get_flag(C_FLAG) || get_flag(N_FLAG) || get_flag(V_FLAG) ||
             get_flag(Z_FLAG)),
           name);
}

static void test_memory_operations() {
  /*
   * Test get_word(address_t addr), set_word(address_t, word_t instruction),
   * get_memory(address_t addr), and set_memory(address_t addr, byte_t value):
   * Test if the return value is in big endian format and initialized to 0
   */

  char *name;
  const address_t STEP = 8192;

  for (address_t addr = 0; addr < MEM_ADDR - STEP; addr += STEP) {
    printf("Address: %d\n", addr);
    /* Testing memory initialization */
    name = "Testing if memory is initialized to 0";
    testword(get_word(addr), 0, name);

    /* Testing set_word() */
    name = "Testing if set_word() works properly";
    word_t test_instr = addr;
    set_word(addr, test_instr);
    testword(get_word(addr), test_instr, name);

    /* Testing get_word() at addresses that are not aligned to 4 bytes */
    name = "Testing get_word() at addresses that are not aligned to 4 bytes";
    set_word(addr, 0x01234567);
    set_word(addr + 4, 0x89abcdef);
    testword(get_word(addr + 2), 0xcdef0123, name);
  }
}

static void test_load_program() {
  /*
   * Test load_program(word_t *buffer, size_t size):
   * Check if the program is loaded into the memory properly starting
   * with address 0 and 4 bytes for each instruction
   */

  /* an example file to load into memory */
  // FILE *fp = fopen("factorial", "r");

  // char *name;
  // uint32_t *buffer = NULL;
  // size_t size;

  // read_binary_file("factorial", &buffer, &size);
  // load_program(buffer, size);

  // /* Testing if the program is correctly loaded in memory */
  // name = "Testing if the program is loaded into the memory";
  // for (int i = 0; i < size; i++) {
  //   testword(get_word(i << 2), buffer[i], name);
  // }

  // free(buffer);
  // fclose(fp);
}

static void test_return_state() {
  free_state();
  print_state(stdout);
  /* print to the standard output for convenience*/
}

int main(void) {
  /* construct state using init_state() */
  init_state();

  /* testing cases */
  test_reg_operations();
  test_flag_operations();
  test_memory_operations();
  test_load_program();
  /* the state will be deconstructed in test_return_state() */
  test_return_state();
}
