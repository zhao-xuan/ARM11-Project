#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "linked_list.h"

#define INIT_PREV_CURR\
  list_node **prev = malloc(sizeof(list_node *)), **curr = malloc(sizeof(list_node *));

#define CHECK_MEM_PREV_CURR(func)\
  if (!prev || !curr) {\
    fprintf(stderr, "Memory allocation in _func_ failed!");\
    return false;\
  }

#define RETURN_IF_ITEM_EXISTS\
  if (traverse(list, prev, curr, item, size)){\
    free(prev);\
    free(curr);\
    return false;\
  }
  
typedef struct list_node list_node; 


/* Structure for node of the linked list */
struct list_node {
  void *item;
  size_t size;
  list_node *next;
};

/* Linked list structure which tracks the head and tail
 * Comparator is a pointer to a function which takes in 
 * 2 objects of ADT similar to the equals function in JAVA
 */ 
struct linked_list {
  list_node *head;
  comparator *cmp;
}; 

/* Free resources allocated to each node */
void free_node(list_node *node) {
  free(node->item);
  free(node);
}

/* Free resources allocated to linked list */
void free_list(linked_list *list) {
  for (list_node *curr = list->head; curr; curr = curr->next) {
    free_node(curr);
  }
  free(list);
}

/* Create a node for the linked list 
 * 
 *  @param:
 *    - item: item to be inserted to the list
 *    - size: size of item in BYTES
 *    - next: pointer to the next node 
 *  
 *  @return: pointer to new node, NULL if anything fails
 */
list_node *create_node(void *item, size_t size, list_node *next) {
  list_node *node = calloc(1, sizeof(list_node));
  if (!node) return NULL;
 
  node->next = next;
  node->size = size;
  node->item = malloc(size);
  memcpy(node->item, item, size);
  
  if (!node->item) {
    free(node);
    return NULL;
  }
  return node;
}

linked_list *create_linked_list(comparator *cmp) {
  linked_list *list = (linked_list *) calloc(1, sizeof(linked_list));
  list->head = create_node(NULL, 0, NULL);
  list->cmp = cmp;

  if (!list || !list->head) free_list(list);
  
  return list;
}

bool find(linked_list *list, void *item, size_t size) {
  if (!item) return false;
  bool found = false;

  for (list_node *curr = list->head; !found && curr; curr = curr->next){
    found = curr->size == size && list->cmp(curr->item, item) == 0;
  }

  return found;
}

bool insert(linked_list *list, void *item, size_t size) {
  if (!item)  return false;
  list_node *curr;

  for (curr = list->head; curr->next; curr = curr->next) {
    /* Duplicate item found */
    if (curr->next->size == size && list->cmp(curr->next->item, item) == 0)
      return false;
  }
  
  list_node *node = create_node(item, size, NULL);
  curr->next = node;
  return true;
}

bool delete(linked_list *list, void *item, size_t size) {
  if (!item)  return false;
  list_node *prev = list->head, *curr = list->head;
  
  for (; curr; prev = curr, curr = curr->next) {
    /* Found item */
    if (curr->size == size && list->cmp(curr->item, item) == 0){
      prev->next = curr->next;
      free_node(curr);
      return true;
    }  
  }
  return false;
}

void *get(linked_list *list, void *item, size_t size) {
  if (!item)  return false;
  list_node *curr;
  
  for (curr = list->head; curr; curr = curr->next) {
    /* Found item */
    if (curr->size == size && list->cmp(curr->item, item) == 0){
      return curr->item;
    }  
  }
  return NULL;
}
