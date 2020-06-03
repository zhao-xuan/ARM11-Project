/*
 * Testing module for utils
 * Run ./utils_test
 */

#define PATH_TO_FILE "testinput"

#include "utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "testutils.h"

int main(void) {
  uint32_t *buffer;
  size_t size;
  char *test_name;

  /* Check that the buffer contains the correct value */
  /* The testinput file contains "abab0990" */
  /* The Newline character should be ignored by read_binary_file */
  test_name = "read_binary_file: Buffer contains the correct value";
  read_binary_file(PATH_TO_FILE, &buffer, &size);
  /* HEX for "abab". Little-endian. */
  uint32_t buf0 = 0x62616261;
  /* HEX for "1234". Little-endian. */
  uint32_t buf1 = 0x34333231;
  testint(buffer[0], buf0, test_name);
  testint(buffer[1], buf1, test_name);

  /* Check that the correct buffer size is returned */
  test_name = "read_binary_file: Correct buffer size";
  testint(size, 2, test_name);

  return 0;
}
