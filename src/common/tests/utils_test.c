/*
 * Testing module for utils
 * Run ./utils_test
 */

#define TEST_INPUT_PATH "testinput"
#define TEST_OUTPUT_PATH "testoutput"
#define ASSEMBLY_INPUT_PATH "assemblyinput"

#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "testutils.h"

int main(void) {
  word_t *input_buffer;
  word_t *output_buffer = NULL;
  size_t size;
  char *test_name;
  /* Testing reading binary file...
   * Check that the buffer contains the correct value
   * The testinput file contains "abab0990"
   * The Newline character should be ignored by read_binary_file 
   */
  test_name = "read_binary_file: Buffer contains the correct value";
  read_binary_file(TEST_INPUT_PATH, &input_buffer, &size);
  /* HEX for "abab". Little-endian. */
  uint32_t buf0 = 0x62616261;
  /* HEX for "0990". Little-endian. */
  uint32_t buf1 = 0x34333231;
  testword(input_buffer[0], buf0, test_name);
  testword(input_buffer[1], buf1, test_name);

  /* Check that the correct buffer size is returned */
  test_name = "read_binary_file: Correct buffer size";
  testint(size, 2, test_name);

  /* Testing writing binary file... 
   * Check if the write oepration has done properly
   * The test output is "11010011", in Big Endian Encoding
   */
  test_name = "write_binary_file: Buffer value has been written to the file";
  output_buffer[0] = 0x1101;
  output_buffer[1] = 0x0011;
  write_binary_file(TEST_OUTPUT_PATH, output_buffer, 2);
  word_t *output_test_buffer = NULL;
  read_binary_file(TEST_OUTPUT_PATH, &output_buffer, &size);
  testword(output_test_buffer[0], output_buffer[0], test_name);
  testword(output_test_buffer[1], output_buffer[1], test_name);
  free(output_buffer);
  free(output_test_buffer);

  /*
   * Testing reading assembly file...
   * Check if assembly file can be read properly
   */
  test_name = "read_assembly_file: Buffer contains the correct assembly codes";
  //char *assembly = "mov r0,#1\nmov r1,#5\nloop:\nmul r2,r1,r0\nmov r0,r2\nsub r1,r1,#1\ncmp r1,#0\nbne loop\nmov r3,#0x100\nstr r2,[r3]";
  char **assembly_buffer = read_assembly_file(ASSEMBLY_INPUT_PATH);
  printf("%s\n", *assembly_buffer);


  return EXIT_SUCCESS;
}
