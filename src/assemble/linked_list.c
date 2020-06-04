#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "linked_list.h"

#define MALLOC_PREV_CURR(func) \
 do { list_node **prev = malloc(sizeof(list_node *));\
      list_node **curr = malloc(sizeof(list_node *));\
    if (!prev || !curr) {\
      fprintf(stderr, "Memory allocation in _func_ failed!");\
      return false;\
   }} while(0)

typedef struct list_node list_node; 

typedef int (comparator) (void *, void*);

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
  list_node *tail;
  comparator *cmp;
}; 

/* Free resources allocated to each node */
void free_node(list_node *node) {
  free(node->item);
  free(node);
}

/* Free resources allocated to linked list */
void free_list(linked_list *list) {
  list_node **curr = &list->head;
  
  while((*curr)->next) {
    list_node *prev = *curr;
    *curr =(*curr)->next;
    free_node(prev);
  }
  free_node(*curr);
  free(list);
}

/* Create a node for the linked list 
 * 
 *  @param:
 *    - item: item to be pushed to the list
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
  list->tail = create_node(NULL, 0, NULL);
  list->cmp = cmp;

  if (!list || !list->head || !list->tail) {
    free_list(list);
  }
  list->head->next = list->tail;
  return list;
}

bool traverse(linked_list *list, list_node **prev, list_node **curr, void *item, size_t size) {
  *curr = list->head;
  bool found = false;
  
  while (!found && (*curr)->next) {
    *prev = *curr;
    *curr = (*curr)->next;
    found = (*curr)->size == size
            && list->cmp(item, (*curr)->item) == 0;
  }
  return found;
}

bool find(linked_list *list, void *item, size_t size) {
  if (!item) return false;

  list_node **prev = malloc(sizeof(list_node *));
  list_node **curr = malloc(sizeof(list_node *));
  
  if (!prev || !curr) {
    fprintf(stderr, "Memory allocation in _find_ failed!");
    return false;
  }
  bool found = traverse(list, prev, curr, item, size);
  free(prev);
  free(curr);

  return found;
}

bool push(linked_list *list, void *item, size_t size) {
  if (!item)  return false;

  list_node **prev = malloc(sizeof(list_node *));
  list_node **curr = malloc(sizeof(list_node *));
  
  if (!prev || !curr) {
    fprintf(stderr, "Memory allocation in _push_ failed!");
    return false;
  }
  
  if (traverse(list, prev, curr, item, size)){
    free(prev);
    free(curr);
    return false; /* Item already exists */
  }

  list_node *node = create_node(item, size, NULL);
  (*prev)->next = node;
  node->next = *curr;
  free(prev);
  free(curr);
  return true;
}

bool pop(linked_list *list, void *item, size_t size) {
  if (!item)  return false;
  
  list_node **prev = malloc(sizeof(list_node *));
  list_node **curr = malloc(sizeof(list_node *));
  
  if (!prev || !curr) {
    fprintf(stderr, "Memory allocation in _pop_ failed!");
    return false;
  }

  if (!traverse(list, prev, curr, item, size)) {
    free(prev);
    free(curr);
    return false; /* Item already exists */
  }
  
  (*prev)->next = (*curr)->next;
  free(prev);
  free_node(*curr);
  free(curr);
   
  return true;
}

