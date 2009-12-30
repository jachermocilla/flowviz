/*---------------------------------------------------------------------\
 * 
 * list.h
 * 
 * Header file for the implementation of the List ADT using 
 * doubly linked lists
 * 
 * @author Joseph Anthony C. Hermocilla
 * @version 1.0
 *
 * This header file can be modified such that other implementations of 
 * the List ADT may be employed, i.e sequential array. This implementation
 * relies heavily on poiniters.
 *  
 *
 * Date				Modification
 * July 28, 2003    Added elementAtIndex function
 * 
\---------------------------------------------------------------------*/


/* Make sure that this header file is included only once */
#ifndef __LIST_H_
#define __LIST_H_

#include "myglobals.h"

typedef struct _ListNode *ListNode;
typedef ListNode ListIterator;
typedef struct _List *List;

/*---------------------------------------------------------------------
 * Declarations for a doubly linked list implementation of the List
 * ADT (implementation specific).
\---------------------------------------------------------------------*/ 
struct _ListNode {
	void *data;	/*Any type of data can be stored using a generic pointer */
	ListNode next;  /*Pointer to the next node */
	ListNode prev;  /*Pointer to the prev node */
};

struct _List {
	int n;		/*The number of nodes that are currently on the list*/
	ListNode head;	/*Pointer to the first element of the list*/
	ListNode tail;	/*Pointer to the last element of the list */
};

/* Utility function to create a new ListNode that contains data*/
ListNode	ListNode_new(void *data);

/*-----End of implementation specific declarations    -----------------*/


/*---------------------------------------------------------------------\ 
 * A General interface for the list ADT
 * You can add additional functions if you wish, i.e. merge(),search()
 * display() and sort().
\---------------------------------------------------------------------*/

/* Creates a new List */
List 		List_new();

/* Deletes a list by freeing all its contents */
void 		List_delete(List *list);

/* Inserts a new element on the list at position pos */
void 		List_insert(List list, ListIterator pos, ElementType data);

/* Removes  an element from the list at position pos*/
void 		List_remove(List list, ListIterator pos);

/*Returns the information stored at position pos */
ElementType 	List_elementAt(List list, ListIterator pos);

/*Changes the information stored at position pos */
void 		List_setElementAt(List list, ListIterator pos,ElementType data);

/* Returns an iterator for the beginning of the list */
ListIterator	List_begin(List list);

/* -------------------------------------------------------------------\
 * Returns an iterator that specifies the end of the list.
 * NOTE: DOES NOT RETURN AN ITERATOR TO THE LAST ELEMENT OF THE LIST
\*-------------------------------------------------------------------*/
ListIterator	List_end(List list);

/*Returns an iterator to the last element of the list */
ListIterator 	List_last(List list);

/* Returns an iterator for the next element after position pos */
ListIterator	List_next(ListIterator pos);

/* Returns an iterator for the previous element before position pos*/
ListIterator	List_prev(ListIterator pos);

/* Returns the number of elements on the list*/
int		List_size(List list);

/* Returns 0 if list is not empty, 1 otherwise */
int		List_isEmpty(List list);

/* Reverses the list */
void		List_reverse(List list);

/* Retrieves the ith element in the list */
ElementType		List_elementAtIndex(List list,int i);

#endif
