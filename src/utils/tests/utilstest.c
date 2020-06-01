/*
 * Testing module for utils.h
 * Make and run ./utilstest
 */

#define PATH_TO_FILE "testinput"
#define PATH_TO_EMPTY "emptyinput"
#define PATH_TO_NONE "PHP_is_the_worst_language"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "testutils.h"

int main(void) {
  uint32_t *buffer;
  size_t size;
  char* test_name;
  int status;

  /* Check that read_binary_file returns SUCCESS when given the correct file */
  test_name = "read_binary_file: Correct return status";
  status = read_binary_file(PATH_TO_FILE, &buffer, &size);
  testint(status, 0, test_name);
  
  /* Check that the buffer contains the correct value */
  /* The testinput file contains "abab0990" */
  /* The Newline character should be ignored by read_binary_file */
  test_name = "read_binary_file: Buffer contains the correct value";
  /* HEX for "abab". Little-endian. */
  uint32_t buf0 = 0x62616261;
  /* HEX for "1234". Little-endian. */
  uint32_t buf1 = 0x34333231;
  testint(buffer[0], buf0, test_name);
  testint(buffer[1], buf1, test_name);
  
  /* Check that the correct buffer size is returned */
  test_name = "read_binary_file: Correct buffer size";
  testint(size, 2, test_name);

  /* Check that an empty file can be read correctly */
  test_name = "read_binary_file: Empty file can be handled correctly";
  status = read_binary_file(PATH_TO_EMPTY, &buffer, &size);
  testint(status, -1, test_name);
  testint(size, 0, test_name);

  /* Check that wrong path can be handled correct */
  test_name = "read_binary_file: Wrong path can be handle correctly";
  status = read_binary_file(PATH_TO_NONE, &buffer, &size);
  testint(status, -1, test_name);


  return 0;
}
