/**-------------------------------------------------------------------------\
 * 
 * list.c
 *
 * 
 * Implementation for the interface of the doubly linked list implementation
 * of the List ADT
 *
 * @author Joseph Anthony C. Hermocilla
 * @version 1.0
 *
\*-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

 /**
 * Returns a new ListNode that contains the passed data. 
 *
 * @param data a generic pointer to whatever data is to be stored on the list
 * @return a ListNode that contains the data
 * @precondition	(data != NULL)
 * @postcondition	a new ListNode is returned
 */
ListNode ListNode_new(void *data){

	ListNode newnode = (ListNode)malloc(sizeof(struct _ListNode));	
	
	assert(data != NULL);
	
	/*make sure that memory was allocated! */
	assert(newnode != NULL);
	
	newnode->data = data;
	newnode->next = newnode->prev = NULL;
	
	return newnode;
}

/**
 * Returns a new List. The list does not use a dummy header, thus
 * special conditions should be manually handled.
 *
 * @param none
 * @return a new List
 * @precondition none
 * @postcondition a new List is returned
 */
List List_new(){
	List list = (List)malloc(sizeof(struct _List));

	/*make sure that memory was allocated*/
	assert(list != NULL);
	
	list->n = 0;
	list->head = NULL;
	list->tail = NULL;

	return list;
}

/**
 * Deletes an entire list. All elements of the list are deleted
 * one by one until none is left.
 *
 * @param	a pointer to the address of the list to be deleted.
 * @return 	none
 * @precondition  (*list != NULL) , list not empty
 * 
 * @postcondition (*list)->head = NULL
 * 		  (*list)->tail = NULL
 * 		  (*list)->n    = 0
 *
 */ 
void List_delete(List *list){

	
	ListIterator p = List_begin(*list);
	assert (*list != NULL);
	
	for (p = List_begin(*list); p != List_end(*list); p = List_next(p)){
		List_remove(*list,p);
	}

	assert((*list)->head == NULL);
	assert((*list)->tail == NULL);
	assert((*list)->n == 0);
	
}

/* Insert item on list based on some criteria*/
ElementType   List_insert_sorted(List list, ListIterator pos, ElementType data, 
        int (*compar)(const void *, const void *))
{

	ListNode newnode = ListNode_new(data);
	
	assert(list != NULL);
	assert(data != NULL);
	
	if (pos == NULL && list->head == NULL){	/*insert at the head */
		list->head = newnode;
		list->tail = newnode;
	}else if (pos != NULL){
		if (pos == list->head){		/*with one node*/
			newnode->next = pos;
			pos->prev = newnode;
			list->head = newnode;
		}else{
			newnode->next = pos;	/*with two or more nodes*/
			pos->prev->next = newnode;
			newnode->prev = pos->prev;
		}
	}else if (pos == NULL){			/*add at the end*/
		list->tail->next = newnode;
		newnode->prev = list->tail;
		list->tail = newnode;
	}

	list->n++;

}


/**
 * Inserts a new item or element on a list. What is passed in this function
 * is the data needed. The node is created inside the function.
 *
 * @param list a List where the new node will be inserted
 * @param pos a ListIterator pointing to where the new node will be inserted
 * @param data the information contained in the node
 * @return none
 * @precondition  	list != NULL
 * 			data != NULL
 * @postcondition	a new node containing data is inserted at position pos
 * 			on the list list. list->n is added one if successful
 * 			insertion.
 *
 */

void List_insert(List list, ListIterator pos, ElementType data){


	ListNode newnode = ListNode_new(data);
	
	assert(list != NULL);
	assert(data != NULL);
	
	if (pos == NULL && list->head == NULL){	/*insert at the head */
		list->head = newnode;
		list->tail = newnode;
	}else if (pos != NULL){
		if (pos == list->head){		/*with one node*/
			newnode->next = pos;
			pos->prev = newnode;
			list->head = newnode;
		}else{
			newnode->next = pos;	/*with two or more nodes*/
			pos->prev->next = newnode;
			newnode->prev = pos->prev;
		}
	}else if (pos == NULL){			/*add at the end*/
		list->tail->next = newnode;
		newnode->prev = list->tail;
		list->tail = newnode;
	}

	list->n++;
	
}

/**
 * Removes the node at position specified by pos.
 *
 * @param list a List from where we will delete
 * @param pos a ListIterator which points to the element to be deleted.
 * @precondition 	list != NULL
 * @postcondition	the element pointed to by pos is removed and 
 * 			list->n is decremented by 1
 * 
 */
void List_remove(List list, ListIterator pos){

	
	assert(list != NULL);
		

	if (list->head == NULL){  /*ignore the call to delete NULL*/
		return;
	}else if (pos != NULL){
		if (pos == list->head){		/* node at head */
			list->head = pos->next;
			if (pos->next != NULL)	/*there are two nodes */
				pos->next->prev = NULL;
			else			/* only one node */
				list->tail = NULL;
			free(pos);
		}else{
			if (list->tail == pos){	/*node is the last*/
				list->tail = pos->prev;
				list->tail->next = NULL;
			}
			else{
				pos->prev->next = pos->next;	/*normal delete*/
				pos->next->prev = pos->prev;
			}
			free(pos);
		}
	}
	list->n--;
		
}

/**
 * Returns the data on the node pointed to by pos.
 *
 * @param list 		a List
 * @param pos  		the node from where the data will be fetched
 * @return 		a void * that points to the data
 * @precondition 	list != NULL
 * @postcondition	the pointer to the element is returned if
 * 			pos is not NULL
 */
ElementType List_elementAt(List list, ListIterator pos){
	assert(list != NULL);
	
	if (pos != NULL)
		return pos->data;
	else 
		return NULL;
}

/**
 * Sets the data on the node pointed to by pos.
 *
 * @param list 		a List
 * @param pos  		the node from of data to change
 * @param data		the new data to place on the node
 * @return 		a void * that points to the data
 * @precondition 	list != NULL
 * @postcondition	the pointer to the element is returned if
 * 			pos is not NULL
 */
void List_setElementAt(List list, ListIterator pos,ElementType data){
	assert(list != NULL);
	assert(pos != NULL);
	pos->data = data;
}

/**
 * Returns an iterator to the beginning of the list
 *
 * @param list a List
 * @return a ListIterator that points to the beginning of the list
 * @precondition	list != NULL
 * @postcondition	an iterator to the beginning of the list is returned
 *
 *
 */
ListIterator List_begin(List list){
	assert(list != NULL);
	return list->head;
}

/**
 * Returns an iterator that specifies the end of the list.
 * NOTE: RETURNS NOT AN ITERATOR TO THE LAST ELEMENT OF THE LIST.  
 *
 * @param list a List
 * @return a ListIterator that specifies the end of the list
 * @precondition	list != NULL
 * @postcondition	an iterator that specifies the end of the list
 * 			is returned.
 *
 */
ListIterator List_end(List list){
	assert(list != NULL);
	return NULL;
}

/**
 * Returns an iterator pointing to the last element of the list
 *
 * @param list a List
 * @return a ListIterator that points to the last element of the list
 * @precondition	list != NULL
 * @postcondition	an iterator that specifies points to the last
 * 			element of the list.
 *
 */
ListIterator List_last(List list){
	assert(list != NULL);
	return list->tail;
}
/**
 * Returns an iterator that specifies the element after pos.
 *
 * @param pos a ListIterator
 * @return a ListIterator that specifies the element after pos
 * @precondition	none
 * @postcondition	a ListIterator that points to the element
 * 			after pos.
 *
 */
ListIterator List_next(ListIterator pos){
	if (pos != NULL)
		return pos->next;
	else	
		return pos;
}


/**
 * Returns an iterator that specifies the element before pos.
 *
 * @param pos a ListIterator
 * @return a ListIterator that specifies the element before pos
 * @precondition	none
 * @postcondition	a ListIterator that points to the element
 * 			before pos.
 *
 */
ListIterator List_prev(ListIterator pos){
	if (pos != NULL)
		return pos->prev;
	else
		return pos;
}

/**
 * Returns the number of elements in the list.
 * 
 * @param list a List
 * @return a ListIterator that specifies the end of the list
 * @precondition	list != NULL
 * @postcondition	size of the list is returned
 *
 */
int List_size(List list){
	assert(list != NULL);
	return list->n;
}


/**
 * Returns a number greater than 0 to specify that the list is
 * not empty.
 *
 * @param list a List
 * @precondition	list != NULL
 *
 */
int List_isEmpty(List list){
	assert(list != NULL);
	if (list->n == 0)
		return 1;
	return 0;
}


/**
 * Reverses the list.
 * 
 *
 * @param list a List
 * @precondition	list != NULL
 *
 */
void List_reverse(List list){
	void *data;
	
	assert(list != NULL);

	if (!List_isEmpty(list)){
		data = List_elementAt(list,List_begin(list));
		List_remove(list,List_begin(list));
		List_reverse(list);
		List_insert(list,List_end(list),data);
	}		
}

/* Retrieves the ith element in the list, returns NULL if not
 * found or i >= n
 * i = 0 -> first element
 *
 * This is not an optimzed version.
 *
 *
 * @param list a list
 * @precondition list != NULL
*/
ElementType		List_elementAtIndex(List list,int i){
	int j = 0;
	ListIterator li;
	
	assert(list != NULL);
	
	if (i >= list->n) return NULL;
	
	li = List_begin(list);
	while ((li != List_end(list)) && (j < i)){
		li = List_next(li);
		j++;
	}
	
	if (li == List_end(list))
		return NULL;
	else
		return li->data;
	
}
