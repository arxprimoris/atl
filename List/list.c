#include <stdlib.h>
#include "list.h"

int List_create(LinkedList* list, int (*elementDestructor)(void*)) {
	if(list == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	list->_firstNode = NULL;
	list->_lastNode  = NULL;
	list->_curNode   = NULL;
	list->_elementDestructor = elementDestructor;

	return(LIST_FUNC_SUCCESS);
}

int List_destroy(LinkedList* list) {
	if(list == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	while(List_chop(list) != LIST_EMPTY){
	}
	return(LIST_FUNC_SUCCESS);
}

int List_append(LinkedList* list, void* data) {
	if(list == NULL || data == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	list->_lastNode = _insertNode(list->_lastNode, NULL, data);
	//  If list was empty we need to initialize all member variables.
	if(list->_firstNode == NULL) {
		list->_firstNode = list->_curNode = list->_lastNode;
	}
	return(LIST_FUNC_SUCCESS);
}

int List_behead(LinkedList* list) {
	if(list == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	if(list->_firstNode == NULL) {
		return(LIST_EMPTY);
	}
	struct _ListNode* newFirstNode = list->_firstNode->next;

	if(list->_curNode == list->_firstNode) {
		list->_curNode = newFirstNode;	// Will be null if firstNode was the last node.
	}
	if(list->_lastNode == list->_firstNode) {
		list->_lastNode = newFirstNode; // Will be null if firstNode was the last node.
	}
	list->_elementDestructor(list->_firstNode->data);
	_removeNode(list->_firstNode);
	list->_firstNode = newFirstNode; 	// You guessed it; will be null if firstNode was last node.

	return(LIST_FUNC_SUCCESS);
}

int List_prepend(LinkedList* list, void* data) {
	if (list == NULL || data == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	list->_firstNode = _insertNode(NULL, list->_firstNode, data);
	//  If list was empty we need to initialize all member variables.
	if(list->_lastNode == NULL) {
		 list->_lastNode = list->_curNode = list->_firstNode;
	}
	return(LIST_FUNC_SUCCESS);
}

int List_chop(LinkedList* list) {
	if(list == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	if(list->_lastNode == NULL) {
		return(LIST_EMPTY);
	}
	struct _ListNode* newLastNode = list->_lastNode->prev;

	if(list->_curNode == list->_lastNode) {
		list->_curNode = newLastNode; 	// Will be null if firstNode was the last node.
	}
	if(list->_firstNode == list->_lastNode) {
		list->_firstNode = newLastNode; // Will be null if firstNode was the last node.
	}
	list->_elementDestructor(list->_lastNode->data);
	_removeNode(list->_lastNode);
	list->_lastNode = newLastNode;		// You guessed it; will be null if firstNode was last node.

	return(LIST_FUNC_SUCCESS);
}

int List_insert(LinkedList* list, void* beforeItem, void* data, int (*elementCompare)(void*, void*)) {
	if(list == NULL || beforeItem == NULL || data == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	struct _ListNode* curCopy = list->_curNode;
	list->_curNode = list->_firstNode;

	int returnVal = List_iteratorTo(list, data, elementCompare);

	if(returnVal == LIST_FUNC_SUCCESS) {
		List_iteratorInsert(list, data);
	}
	list->_curNode = curCopy;
	return(returnVal);
}

int List_remove(LinkedList* list, void* data, int (*elementCompare)(void*, void*)) {
	if (data == NULL || list == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	struct _ListNode* curCopy = list->_curNode;
	list->_curNode = list->_firstNode;

	int returnVal = List_iteratorTo(list, data, elementCompare);

	if(returnVal == LIST_FUNC_SUCCESS) {
		List_iteratorRemove(list);
	}
	list->_curNode = curCopy;
	return(returnVal);
}

int List_replace(LinkedList* list, void* data) {
	return(LIST_FUNC_SUCCESS);
}

void* List_iteratorBegin(LinkedList* list) {
	if(list == NULL || list->_firstNode == NULL) {
		return(NULL);
	}
	list->_curNode = list->_firstNode;

	return(list->_curNode->data);
}

void* List_iteratorEnd(LinkedList* list) {
	if(list == NULL || list->_lastNode == NULL) {
		return(NULL);
	}
	list->_curNode = list->_lastNode;

	return(list->_curNode->data);
}

void* List_iteratorNext(LinkedList* list) {
	if(list == NULL || list->_curNode == NULL) {
		return(NULL);
	}
	if(list->_curNode->next == NULL) {
		return(NULL);
	}
	list->_curNode = list->_curNode->next;

	return(list->_curNode->data);
}

void* List_iteratorPrev(LinkedList* list) {
	if(list == NULL || list->_curNode == NULL) {
		return(NULL);
	}
	if(list->_curNode->prev == NULL) {
		return(NULL);
	}
	list->_curNode = list->_curNode->prev;

	return(list->_curNode->data);
}

int List_iteratorTo(LinkedList* list, void* data, int (*elementCompare)(void*, void*)) {
	if(list ==  NULL || data == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	while(list->_curNode != NULL) {
		if(elementCompare != NULL) {
			if(elementCompare(data, list->_curNode->data) == 0) { // Zero means a match
				return(LIST_FUNC_SUCCESS);
			}
		}
		else if(data == list->_curNode->data) {	// User pointer address if no compare
			return(LIST_FUNC_SUCCESS);			// function was provided.
		}
		list->_curNode = list->_curNode->next;
	}
	// Item not found, and we do not want to leave _curNode pointing to NULL.
	list->_curNode = list->_lastNode;
	return(LIST_ITEM_NOT_FOUND);
}

int List_iteratorInsert(LinkedList* list, void* data) {
	if(list == NULL || data == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	if(list->_curNode == list->_firstNode) {
		return(List_prepend(list, data));
	}
	_insertNode(list->_curNode->prev, list->_curNode, data);
	return(LIST_FUNC_SUCCESS);
}

int List_iteratorRemove(LinkedList* list) {
	if(list == NULL) {
		return(LIST_ERR_NULL_ARG);
	}
	if(list->_curNode == NULL) {
		return(LIST_EMPTY);
	}
	if(list->_curNode == list->_firstNode) {
		return(List_behead(list) );
	}
	else if(list->_curNode == list->_lastNode) {
		return(List_chop(list) );
	}
	else {
		struct _ListNode* newCurNode = list->_curNode->next;
		if(newCurNode == NULL) {
			newCurNode = list->_curNode->prev;
		}
		list->_elementDestructor(list->_curNode->data);
		_removeNode(list->_curNode);
		list->_curNode = newCurNode;
	}
	return(LIST_FUNC_SUCCESS);
}

int List_iteratorReplace(LinkedList* list, void* data) {
	return(LIST_FUNC_SUCCESS);
}

void* List_iterator(LinkedList* list) {
	if(list == NULL || list->_curNode == NULL) {
		return(NULL);
	}
	return(list->_curNode->data);
}

void* List_first(LinkedList* list) {
	if(list == NULL || list->_firstNode == NULL) {
		return(NULL);
	}
	return(list->_firstNode->data);
}

void* List_last(LinkedList* list) {
	if(list == NULL || list->_lastNode == NULL) {
		return(NULL);
	}
	return(list->_lastNode->data);
}

int _removeNode(struct _ListNode* node) {
	if(node->next != NULL) {
		node->next->prev = node->prev;
	}
	if(node->prev != NULL) {
		node->prev->next = node->next;
	}
	free(node);

	return(0);
}

void* _insertNode(struct _ListNode* prevNode, struct _ListNode* nextNode, void* data) {
	struct _ListNode* newNode = (struct _ListNode*) malloc(sizeof(struct _ListNode));

	if(prevNode != NULL) {
		prevNode->next = newNode;
	}
	if(nextNode != NULL) {
		nextNode->prev = newNode;
	}
	newNode->next = nextNode;
	newNode->prev = prevNode;
	newNode->data = data;

	return(newNode);
}
