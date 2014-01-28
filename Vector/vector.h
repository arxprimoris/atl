#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////////////////////////
//  Vector function return values
/////////////////////////////////////////////////////////////////////////////////////////
#define VECTOR_EMPTY			  1	// Vector is empty
#define VECTOR_FUNC_SUCCESS		  0	// No error
#define VECTOR_ERR_NULL_ARG		 -1	// Required pointer argument is NULL
#define VECTOR_ERR_INVALID_ARG	 -2	// An invalid value has been passed to function
#define VECTOR_ERR_ALLOCATION	 -3	// Vector capacity resize has failed
#define VECTOR_ERR_OUT_OF_BOUNDS -4	// Attempted to access an index out of bounds of the vector

/////////////////////////////////////////////////////////////////////////////////////////
// How much more memory is allocated (as a factor) over vector size when a vector data
// reallocation is required.
/////////////////////////////////////////////////////////////////////////////////////////
#define CAPACITY_FACTOR	2

/////////////////////////////////////////////////////////////////////////////////////////
//  Vector is the client-side data structure for a vector. The
//  members within Vector will be managed with the Vector_...
//  functions and do not require client interaction.
//  Member - _data: 		Pointer to start of memory holding contiguous elements.
//  Member - _size:			Number of elements.
//  Member - _capacity:		Size of total allocated memory, in bytes, pointed to by _data.
//  Member - _elementSize:	Size, in bytes, of each individual element in memory.
//  Member - _elementDestructor:	Function pointer to client-side element destructor.
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct _Vector {
	long _size;
	long _capacity;
	int _elementSize;
	void* _data;
	int (*_elementDestructor)(void*);
} Vector;

/////////////////////////////////////////////////////////////////////////////////////////
//  Initializes and allocates memory for vector.
//
//  Arg - vector: 			 Pointer to the vector which is being created.
//  Arg - capacity: 		 Desired initial capacity allocated in bytes. This does not 
//							 effect inital size, which will be 0.
//  Arg - elementSize:		 Size, in bytes, of each individual element in memory.
//  Arg - elementDestructor: Function pointer to client-side element destructor.
//
//  Returns: VECTOR_... #defined above.
/////////////////////////////////////////////////////////////////////////////////////////
int Vector_create(Vector* vector, long capacity, int elementSize, int (*elementDestructor)(void*));

/////////////////////////////////////////////////////////////////////////////////////////
//  Iterates though vector freeing all element's memory by using the destructor passed to
//  Vector_create, then frees the vector data pointer itself.
//
//  Arg - vector: Pointer to the vector which is being destroyed.
//
//  Returns: VECTOR_... #defined above.
/////////////////////////////////////////////////////////////////////////////////////////
int Vector_destroy(Vector* vector);

/////////////////////////////////////////////////////////////////////////////////////////
//	Resizes (and if neccessary allocates additional space for) vector. If new size is 
//  smaller, old elements passed new size will be deallocated. If knew size is bigger,
//  new elements will be initialized to initData.
//
//  Arg - vector:	Pointer to the vector which is being resized.
//  Arg - initData:	Pointer to data that will be used to "fill in" new elements. Should
//					NOT contain dynamically allocated memory.
//  Arg - size:		Desired size of vector.
//
//  Returns: VECTOR_... #defined above.
//
//  Note: If new size is smaller than old size, initData is ignored and the area of memory
//        corresponding to the old elements will be zeroed out. If new size is bigger
//		  than old size, initData must not be NULL. Additionally, because initData is 
//		  merely appended to vector until vector is of size, initData should not contain
//		  dynamically allocated memory because the destructor would then be called on 
//		  the same pointer repeatedly
/////////////////////////////////////////////////////////////////////////////////////////
int Vector_resize(Vector* vector, const void* initData, long size);

/////////////////////////////////////////////////////////////////////////////////////////
//  Allocates or reallocates vector data size.
//
//  Arg - vector:	Pointer to the vector which is being reallocated.
//  Arg - capacity:	Desired size of memory allocated, in bytes, for vector data.
//
//  Returns: VECTOR_... #defined above.
//
//	Note: This function is called by other Vector functions when needed. All memory
//		  allocated is initialized to 0.
/////////////////////////////////////////////////////////////////////////////////////////
int Vector_resizeCapacity(Vector* vector, long capacity);

/////////////////////////////////////////////////////////////////////////////////////////
//  Returns amount of memory allocated for vector.
//
//  Arg - vector: Pointer to the vector.
//
//  Returns: Current memory allocated, in bytes, for vector.
/////////////////////////////////////////////////////////////////////////////////////////
long Vector_capacity(const Vector* vector);

/////////////////////////////////////////////////////////////////////////////////////////
//  Returns number of elements in vector.
//
//  Arg - vector: Pointer to the vector.
//
//  Returns: Number of elements in vector.
/////////////////////////////////////////////////////////////////////////////////////////
long Vector_size(const Vector* vector);

/////////////////////////////////////////////////////////////////////////////////////////
//  Returns pointer to vector data.
//
//  Arg - vector: Pointer to the vector.
//
//  Returns: Pointer to beginning of memory allocated for vector.
/////////////////////////////////////////////////////////////////////////////////////////
void* Vector_array(const Vector* vector);

/////////////////////////////////////////////////////////////////////////////////////////
//  Returns pointer to element at index (after an in-bounds check).
//
//  Arg - vector: Pointer to the vector.
//  Arg - index: Element to fetch.
//
//  Returns: Pointer to element at index.
/////////////////////////////////////////////////////////////////////////////////////////
void* Vector_get(const Vector* vector, long index);

/////////////////////////////////////////////////////////////////////////////////////////
//  Overwrites element at index with element data passed as argument.
//
//  Arg - vector: Pointer to the vector being altered.
//  Arg - data:	  New element to set at index.
//  Arg - index:  index position of element to be overwritten by data.
//
//  Returns: VECTOR_... #defined above.
//
//	Note: The client-side destructor is called on the element at index before it is 
//        overwritten.
/////////////////////////////////////////////////////////////////////////////////////////
int Vector_set(Vector* vector, const void* data, long index);

/////////////////////////////////////////////////////////////////////////////////////////
//  Returns pointer to first element in vector.
//
//  Arg - vector: Pointer to the vector.
//
//  Returns: Pointer to first element.
/////////////////////////////////////////////////////////////////////////////////////////
void* Vector_first(const Vector* vector);

/////////////////////////////////////////////////////////////////////////////////////////
//  Returns pointer to last element in vector.
//
//  Arg - vector: Pointer to the vector.
//
//  Returns: Pointer to last element.
/////////////////////////////////////////////////////////////////////////////////////////
void* Vector_last(const Vector* vector);

/////////////////////////////////////////////////////////////////////////////////////////
//  Appends an element to the end of the vector.
//
//  Arg - vector: Pointer to the vector which will be appended.
//  Arg - data:	  New element which will be appended to the end of vector.
//
//  Returns: VECTOR_... #defined above.
//
//  Note: Successful execution will result in the vector size being incremented by one.
//        Vector capacity may be reallocated if necessary.
/////////////////////////////////////////////////////////////////////////////////////////
int Vector_append(Vector* vector, const void* data);

/////////////////////////////////////////////////////////////////////////////////////////
//  Removes an element from the end of vector.
//
//  Arg - vector: Pointer to the vector from which the last element will be removed.
//
//  Returns: VECTOR_... #defined above.
//
//  Note: The client-side destructor will be called on the last element of vector, and 
//		  the correseponding region of memory which the last element occupied will be
//		  zeroed out. Successful excecution will result in the vector size being
//		  decremented by one.
/////////////////////////////////////////////////////////////////////////////////////////
int Vector_chop(Vector* vector);

/////////////////////////////////////////////////////////////////////////////////////////
//  Inserts a new element at index.
//
//  Arg - vector: Pointer to vector in which a new element will be inserted.
//	Arg - data:	  New element to be inserted.
//  Arg - index:  Position within vector to insert new element.
//
//  Returns: VECTOR_... #defined above.
//
//	Note: Elements from index to size, inclusive, are shifted right by memmove, then new 
//		  element is inserted at index. Successful execution will result in the vector 
//        size being incremented by one. Vector capacity may be reallocated if necessary.
/////////////////////////////////////////////////////////////////////////////////////////
int Vector_insert(Vector* vector, const void* data, long index);

/////////////////////////////////////////////////////////////////////////////////////////
//  Removes the element at index from vector.
//
//  Arg - vector: Pointer to vector from which the element will be removed.
//  Arg - index:  Position of element within vector that will be removed.
//
//  Returns: VECTOR_... #defined above.
//
//  Note: Client-side destructor is called on element at index, then elements from 
//		  index + 1 to size, inclusive, are shifted left by memmove. The memory
//		  corresponding to the last element in the vector before the shift is zeroed out.
//		  Successful execution will result in the vector size being decremented by one.
/////////////////////////////////////////////////////////////////////////////////////////
int Vector_remove(Vector* vector, long index);

#endif