#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "linked_list.h"
#include "testutils.h"

size_t item_size;
char *item;
char *test_name;


#define TEST_INSERT(sign)\
  testbool(sign push(list, item, item_size), test_name)
  
#define TEST_FIND(sign)\
  testbool(sign find(list, item, item_size), test_name)

#define TEST_REMOVE(sign)\
  testbool(sign pop(list, item, item_size), test_name)


/* The linkedlist should be ADTs but here I am only testing for
 * item as a string at the moment
 */
int main() {
  linked_list *list = create_linked_list(&strcmp);
  
  /* Initialise list correctly */
  test_name = "List initialised correctly";
  testbool(list, test_name);

  /* Find an element in empty list*/
  test_name = "Find element in empty list should return 'False'";
  item_size = strlen(test_name);
  TEST_FIND(!);

  /* New inserts */
  test_name = "Inserting 'First' should return 'True'";
  item = "First";
  item_size = strlen(item);
  TEST_INSERT();
  
  /* Repeated inserts */
  test_name = "Inserting 'First' again must return 'False'";
  TEST_INSERT(!);
 
  /* Remove existing element */
  test_name = "Removing 'First' should return 'True'";
  TEST_REMOVE();
  
  /* 'First' should no longer exists */
  test_name = "'First' should not be found anymore";
  TEST_FIND(!);

  /* Second removal of 'First' */
  test_name = "Trying to remove 'First' again should return 'False'";
  TEST_REMOVE(!);

  /* Deallocate memory */
  free_list(list);
}
