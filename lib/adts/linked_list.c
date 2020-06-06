#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include "linked_list.h"

typedef struct list_node list_node; 


/* Structure for node of the linked list */
struct list_node {
  void *key;
  void *value;
  list_node *next;
};

/* Linked list structure which tracks the head and tail
 * Comparator is a pointer to a function which takes in 
 * 2 objects of ADT similar to the equals function in JAVA
 */ 
struct linked_list {
  list_node *head;
  comparator cmp;
}; 

/* Free resources allocated to each node */
void free_node(list_node *node) {
  free(node->key);
  free(node->value);
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
 *    - key: key of the node
 *    - key_size: size of of key
 *    - value: value held by the node
 *    - next: pointer to the next node 
 *  
 *  Note:
 *    - the node makes a copy of the key and stores
 *      the pointer to the value
 *  @return: pointer to new node, NULL if anything fails
 */
list_node *create_node(void *key, size_t key_size, void *value, list_node *next) {
  list_node *node = calloc(1, sizeof(list_node));
  if (!node) return NULL;
 
  node->next = next;
  node->value = value;
  node->key= malloc(key_size);
  memcpy(node->key, key, key_size);
  
  if (!node->key) {
    free(node);
    return NULL;
  }
  return node;
}

linked_list *create_linked_list(comparator cmp) {
  linked_list *list = (linked_list *) calloc(1, sizeof(linked_list));
  list->head = create_node(NULL, 0, NULL, NULL);
  (list->cmp) = cmp;

  if (!list || !list->head) free_list(list);
  
  return list;
}

bool contains(linked_list *list, void *key) {
  if (!key) return false;
  bool found = false;

  for (list_node *curr = list->head; !found && curr; curr = curr->next){
    found = (list->cmp)(curr->key, key) == 0;
  }

  return found;
}

bool insert(linked_list *list, void *key, size_t key_size, void *value) {
  if (!key)  return false;
  list_node *curr;

  for (curr = list->head; curr->next; curr = curr->next) {
    /* Duplicate key found */
    if ((list->cmp)(curr->next->key, key) == 0)
      return false;
  }
  
  list_node *node = create_node(key, key_size, value, NULL);
  curr->next = node;
  return true;
}

bool delete(linked_list *list, void *key) {
  if (!key)  return false;
  list_node *prev = list->head, *curr = list->head;
  
  for (; curr; prev = curr, curr = curr->next) {
    /* Found key */
    if ((list->cmp)(curr->key, key) == 0){
      prev->next = curr->next;
      free_node(curr);
      return true;
    }  
  }
  return false;
}

void *find(linked_list *list, void *key) {
  if (!key)  return false;
  list_node *curr;
  
  for (curr = list->head; curr; curr = curr->next) {
    /* Found key */
    if ((list->cmp)(curr->key, key) == 0){
      return curr->value;
    }  
  }
  return NULL;
}

