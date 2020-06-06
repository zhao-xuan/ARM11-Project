#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "table.h"
#include "testutils.h"


#define TEST_INSERT(sign)\
  testbool(sign insert_symbol(table, symbol), test_name)

#define TEST_GET(expected)\
  testword(*((int *) get_literal(table, symbol->label)), expected, test_name)

int main() {
  
  symbol_t *symbol = (symbol_t *) malloc(sizeof(symbol_t));
  char *test_name;
  symbol_table_t *table;
  int value = 0;
  symbol->literal = &value;
  
  /* Table should be created */
  test_name = "Table should be created successfully";
  table = create_table();
  testbool(table, test_name);


  /* Symbol should be added correctly */
  test_name = "First symbol should be added correctly";
  symbol->label = "Foo";
  TEST_INSERT();

  /* Adding the same symbol again */
  test_name = "Adding the same symbol should not modify the table";
  TEST_INSERT(!);

  
  /* New symbol should be added correctly */
  test_name = "Second symbol with the same literal should still be added";
  symbol->label = "Bar";
  value = 1234;
  TEST_INSERT();
  
  /* New symbol with same label should be discarded */
  test_name = "Third symbol should be with the same label as the first should not be added";
  value = 0;
  symbol->label = "Foo";
  TEST_INSERT(!);
  
  /* New symbol should be added correctly */
  test_name = "Symbol to test for 'get' added correctly";
  symbol->label = "Label";
  TEST_INSERT();
  
  /* Get literal using label */
  test_name = "Get literal of a label works correctly";
  symbol->label = "Label";
  TEST_GET(123456);

  free_table(table);
  free(symbol);
  return 0;
}
