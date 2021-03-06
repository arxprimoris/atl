#ifndef _LIST_H_
#define _LIST_H_

/////////////////////////////////////////////////////////////////////////////////////////
//  Vector function return values
/////////////////////////////////////////////////////////////////////////////////////////
#define LIST_ITEM_NOT_FOUND		 2	// Item not found during a search or insert/remove
#define LIST_EMPTY			 	 1	// List is empty
#define LIST_FUNC_SUCCESS	 	 0	// No error
#define LIST_ERR_NULL_ARG	 	-1	// Required pointer argument is NULL
#define LIST_ERR_INVALID_ARG 	-2	// An invalid value has been passed to function
#define LIST_ERR_ALLOCATION	 	-3	// List capacity resize has failed

/////////////////////////////////////////////////////////////////////////////////////////
//  _ListNode is the internal atom of data used within the linked
//  list. This data structure will be managed within the List_...
//  functions and do not require client interaction. Members should
//  be self evident.
/////////////////////////////////////////////////////////////////////////////////////////
struct _ListNode
{
	struct _ListNode* next;
	struct _ListNode* prev;
	void* data;
	int (*elementDestructor)(void*);
	int test;
};

/////////////////////////////////////////////////////////////////////////////////////////
//  LinkedList is the client-side data structure for a linked list.
//  The members within LinkedList will be managed with the List_...
//  functions and do not require client interaction.
//  Member - _firstNode: Will always point to head node.
//  Member - _lastNode:  Will always point to tail node.
//  Member - _curNode:   Internal iterator node. Its state will vary
//                       depending on various List_... function calls.
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct _LinkedList
{
	struct _ListNode* _firstNode;
	struct _ListNode* _lastNode;
	struct _ListNode* _curNode;
	int (*_elementDestructor)(void*);
}LinkedList;

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
int List_create(LinkedList* list, int (*elementDestructor)(void*));

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
int List_destroy(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
//  List_append links new data to the end of a linked list
//  Arg - LinkedList: The linked list to append.
//  Arg - item: The data to be appened to the end of the list. It is
//              the client responsibility to allocate and initialize
//				item.
//  Returns: Pointer to the data appended on success else NULL.
//  Note: If list is empty, the first, last and iterator node will
//        point to the newly added node.
/////////////////////////////////////////////////////////////////////////////////////////
int List_append(LinkedList* list, void* data);

/////////////////////////////////////////////////////////////////////////////////////////
// Points the head node to the node next to the head node in the
// list, and destroys the current head node.
//  Arg - list: The list to be beheaded.
//  Returns: 1 if success, else 0.
//  Note: If beheading removes the last node in the list, the head,
//        tail and iterator nodes will be NULL. If the iterator node
//        pointed to the old head, the iterator will point to the
//        new head on return.
/////////////////////////////////////////////////////////////////////////////////////////
int List_behead(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
//  List_prepend links new data to the start of a linked list
//  Arg - LinkedList: The linked list to append.
//  Arg - item: The data to be prepended to the start of the list.
//              It is the client responsibility to allocate and
//				initialize item.
//  Returns: Pointer to the data prepended on success else NULL.
//  Note: If list is empty, the first, last and iterator node will
//        point to the newly added node.
/////////////////////////////////////////////////////////////////////////////////////////
int List_prepend(LinkedList* list, void* data);

/////////////////////////////////////////////////////////////////////////////////////////
// Points the tail node to the node previous to the tail node in the
// list and destroys the current tail node.
//  Arg - list: The list to be chopped.
//  Returns: 1 if success, else 0.
//  Note: If chopping removes the last node in the list, the tail,
//        head and iterator nodes will be NULL. If the iterator node
//        pointed to the old tail, the iterator will point to the
//        new tail on return.
/////////////////////////////////////////////////////////////////////////////////////////
int List_chop(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
//  TODO: Use compare function for data comparison instead of pointer
/////////////////////////////////////////////////////////////////////////////////////////
int List_insert(LinkedList* list, void* beforeItem, void* data, int (*elementCompare)(void*, void*));

/////////////////////////////////////////////////////////////////////////////////////////
//  TODO: Use compare function for data comparison instead of pointer
/////////////////////////////////////////////////////////////////////////////////////////
int List_remove(LinkedList* list, void* data, int (*elementCompare)(void*, void*));

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
int List_replace(LinkedList* list, void* replaceItem, void* data);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void* List_iteratorBegin(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void* List_iteratorEnd(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void* List_iteratorNext(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void* List_iteratorPrev(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
// TODO: Finish function
/////////////////////////////////////////////////////////////////////////////////////////
int List_iteratorTo(LinkedList* list, void* data, int (*elementCompare)(void*, void*));

/////////////////////////////////////////////////////////////////////////////////////////
// TODO: Finish function
/////////////////////////////////////////////////////////////////////////////////////////
int List_iteratorInsert(LinkedList* list, void* data);

/////////////////////////////////////////////////////////////////////////////////////////
// TODO: Finish function
/////////////////////////////////////////////////////////////////////////////////////////
int List_iteratorRemove(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
int List_iteratorReplace(LinkedList* list, void* data);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void* List_iterator(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void* List_first(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void* List_last(LinkedList* list);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
void* _insertNode(struct _ListNode* prevNode, struct _ListNode* nextNode, void* data);

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
int _removeNode(struct _ListNode* node);

#endif
