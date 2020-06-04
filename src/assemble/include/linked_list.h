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
 *    - data: pointer to data to find 
 *    - size: size of data
 *  @return: true iff data is found
 */
bool find(linked_list *list, void *data, size_t size);

/*  
 *  Add an data at the end of the linked list 
 *  @param: as above
 *  @return: true iff the list has been modified
 */
bool insert(linked_list *list, void *data, size_t size);

/* Remove an data from linked list */
bool remove(linked_list *list, void *data, size_t size); 

/* Freee memory allocated to linked_list */
void free_list(linked_list *list);
#endif
