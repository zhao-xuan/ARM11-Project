/*
 *  Implementation of a generic linked list to get started on part 2  
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stddef.h>

typedef struct linked_list linked_list;

typedef int (*comparator) (void *, void*);

/*  
 *  Create a generic linked list 
 *  @param:
 *    - cmp: comparator for 2 objects of type of keys in the list
 *  @return: 
 *    - pointer to linked list on success
 *    - NULL if anything fails
 */
linked_list *create_linked_list(comparator cmp);

/* 
 *  Find an element in a given linked list 
 *  @param: 
 *    - list: pointer to linked_list 
 *    - key: pointer to key to find 
 *  @return: true iff key is found
 */
bool contains(linked_list *list, void *key);

/*  
 *  Add an item at the end of the linked list 
 *  @param: as above
 *      - key_size: size of the key in BYTES 
 *  @return: true iff the list has been modified
 */
bool insert(linked_list *list, void *key, size_t size, void *value);

/* Remove an key from linked list */
bool delete(linked_list *list, void *key); 

/* Free memory allocated to linked_list */
void free_list(linked_list *list);

/* Returns key if present, NULL otherwise */
void *find(linked_list *list, void *key);
#endif
