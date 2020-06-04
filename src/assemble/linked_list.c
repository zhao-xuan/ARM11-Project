#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#include "linked_list.h"

typedef struct list_node list_node; 

struct list_node {
  void *item;
  size_t item_size;
  list_node *next;
};

struct linked_list {
  list_node *head;
  list_node *tail;
}; 

void free_node(list_node *node) {
  free(node->item);
  free(node);
}

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

list_node *create_node(void *item, size_t size, list_node *next) {
  list_node *node = calloc(1, sizeof(list_node));
  node->item_size = size;
  node->next = next;
  node->item = malloc(size);
  memcpy(node->item, item, size);
  return node;
}

linked_list *create_linked_list() {
  linked_list *list = (linked_list *) calloc(1, sizeof(linked_list));
  list->head = create_node(NULL, 0, NULL);
  list->tail = create_node(NULL, 0, NULL); 

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
    found = size == (*curr)->item_size 
            && memcmp((*curr)->item, item, size) == 0;
  }
  return found;
}

bool find(linked_list *list, void *item, size_t size) {
  if (!item) return false;

  list_node **prev = malloc(sizeof(list_node *));
  list_node **curr = malloc(sizeof(list_node *));
  bool found = traverse(list, prev, curr, item, size);
  free(prev);
  free(curr);

  return found;
}

bool insert(linked_list *list, void *item, size_t size) {
  if (!item)  return false;

  list_node **prev = malloc(sizeof(list_node *));
  list_node **curr = malloc(sizeof(list_node *));
  
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

bool remove(linked_list *list, void *item, size_t size) {
  if (!item)  return false;
  
  list_node **prev = malloc(sizeof(list_node *));
  list_node **curr = malloc(sizeof(list_node *));
  
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

