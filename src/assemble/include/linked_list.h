/*
 *  Implementation of a generic linked list to get started on part 2  
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct linked_list linked_list;

/*  
 *  Create a generic linked list 
 *  @return: 
 *    - pointer to linked list on success
 *    - NULL if anything fails
 */
linked_list *create_linked_list();

/* 
 *  Find an element in a given linked list 
 *  @param: 
 *    - list: pointer to linked_list 
 *    - item: pointer to item to find 
 *    - size: size of item
 *  @return: true iff item is found
 */
bool find(linked_list *list, void *item, size_t size);

/*  
 *  Add an item at the end of the linked list 
 *  @param: as above
 *  @return: true iff the list has been modified
 */
bool push(linked_list *list, void *item, size_t size);

/* Remove an item from linked list */
bool pop(linked_list *list, void *item, size_t size); 

/* Free memory allocated to linked_list */
void free_list(linked_list *list);
#endif
