/*
 * Testing module for utils
 * Run ./utils_test
 */

#define TEST_INPUT_PATH "testinput"
#define TEST_OUTPUT_PATH "testoutput"
#define ASSEMBLY_INPUT_PATH "assemblyinput.s"

#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "testutils.h"

int main(void) {
  uint32_t *input_buffer;
  size_t size;
  char *test_name;
  /* Testing reading binary file...
   * Check that the buffer contains the correct value
   * The testinput file contains "abab1234"
   * The Newline character should be ignored by read_binary_file
   */
  test_name = "read_binary_file: Buffer contains the correct value";
  read_binary_file(TEST_INPUT_PATH, &input_buffer, &size);
  /* HEX for "abab". Little-endian. */
  uint32_t buf0 = 0x62616261;
  /* HEX for "1234". Little-endian. */
  uint32_t buf1 = 0x34333231;
  testint(input_buffer[0], buf0, test_name);
  testint(input_buffer[1], buf1, test_name);

  // /* Check that the correct buffer size is returned */
  // test_name = "read_binary_file: Correct buffer size";
  // testint(size, 2, test_name);

  /* Testing writing binary file...
   * Check if the write oepration has done properly
   * The test output is "11010011", in Big Endian Encoding
   */
  test_name = "write_binary_file: Buffer value has been written to the file";
  uint32_t output = 0b11010011;
  write_binary_file(TEST_OUTPUT_PATH, &output, 1);
  uint32_t *output_buffer;
  read_binary_file(TEST_OUTPUT_PATH, &output_buffer, &size);
  testword(*output_buffer, output, test_name);

  free(output_buffer);

  /*
   * Testing reading assembly file...
   * Check if assembly file can be read properly
   */
  test_name = "read_assembly_file: Buffer contains the correct assembly codes";
  char assembly[10][15] = {
      "mov r0,#1",    "mov r1,#5", "loop:",    "mul r2,r1,r0",  "mov r0,r2",
      "sub r1,r1,#1", "cmp r1,#0", "bne loop", "mov r3,#0x100", "str r2,[r3]"};
  char **assembly_buffer = read_assembly_file(ASSEMBLY_INPUT_PATH);
  for (int i = 0; assembly_buffer[i]; i++) {
    teststring(assembly_buffer[i], assembly[i], test_name);
    free(assembly_buffer[i]);
  }
  free(assembly_buffer);

  return EXIT_SUCCESS;
}
