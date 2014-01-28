#include "vector.h"

int Vector_create(Vector *vector, long capacity, int elementSize, int (*elementDestructor)(void*)) {
	if(vector == NULL) {
		return(VECTOR_ERR_NULL_ARG);
	}
	if(capacity < 1 || elementSize < 1) {
		return(VECTOR_ERR_INVALID_ARG);
	}
	vector->_data = NULL;
	vector->_size = 0;
	vector->_capacity = 0;
	vector->_elementSize = elementSize;
	vector->_elementDestructor = elementDestructor;

	Vector_resizeCapacity(vector, capacity);

	return(VECTOR_FUNC_SUCCESS);
} 

int Vector_destroy(Vector* vector) {
	if(vector == NULL) {
		return(VECTOR_ERR_NULL_ARG);
	}
	while(Vector_chop(vector) != VECTOR_EMPTY) {
	} // vector->_size now 0
	free(vector->_data);
	vector->_data = NULL;
	vector->_capacity = 0;
	vector->_elementSize = 0;
	vector->_elementDestructor = NULL;

	return(VECTOR_FUNC_SUCCESS);
}

int Vector_resize(Vector* vector, const void* initData, long size) {
	if(vector == NULL || (initData == NULL && vector->_size < size)) {
		return(VECTOR_ERR_NULL_ARG);
	}
	if(size < 1) {
		return(VECTOR_ERR_INVALID_ARG);
	}
	if(size > vector->_capacity) {
		Vector_resizeCapacity(vector, size * CAPACITY_FACTOR);
	}
	while(vector->_size > size) {
		Vector_chop(vector);
	}
	while(vector->_size < size) {
		Vector_append(vector, initData);
	}
	return(VECTOR_FUNC_SUCCESS);
}

int Vector_resizeCapacity(Vector* vector, long capacity) {
	if(vector == NULL) {
		return(VECTOR_ERR_NULL_ARG);
	}
	if(capacity <= vector->_capacity) {
		return(VECTOR_ERR_INVALID_ARG);
	}
	int oldCapacity = vector->_capacity;
	void* newData = realloc(vector->_data, capacity * vector->_elementSize);

	if(newData == NULL) {
		return(VECTOR_ERR_ALLOCATION);
	}
	vector->_data = newData;
	vector->_capacity = capacity;
	memset(vector->_data + (oldCapacity * vector->_elementSize), '\0', (capacity - oldCapacity) * vector->_elementSize);
	
	return(VECTOR_FUNC_SUCCESS);
}

long Vector_capacity(const Vector* vector) {
	if(vector == NULL) {
		return(VECTOR_ERR_NULL_ARG);
	}
	return(vector->_capacity);
}

long Vector_size(const Vector* vector) {
	if(vector == NULL) {
		return(VECTOR_ERR_NULL_ARG);
	}
	return(vector->_size);
}

void* Vector_array(const Vector* vector) {
	if(vector == NULL) {
		return(NULL);
	}
	return(vector->_data);
}

void* Vector_get(const Vector* vector, long index) {
	if(vector == NULL) {
		return(NULL);
	}
	if(index < 0 || index >= vector->_size) {
		return(NULL);
	}
	return(vector->_data + (index * vector->_elementSize) );
}

int Vector_set(Vector* vector, const void* data, long index) {
	if(vector == NULL || data == NULL) {
		return(VECTOR_ERR_NULL_ARG);
	}
	if(index < 0 || index >= vector->_size) {
		return(VECTOR_ERR_OUT_OF_BOUNDS);
	}
	if(vector->_elementDestructor != NULL) {
		vector->_elementDestructor(vector->_data + ( index * vector->_elementSize) );
	}
	memcpy(vector->_data + (index * vector->_elementSize), data, vector->_elementSize);

	return(VECTOR_FUNC_SUCCESS);
}

void* Vector_first(const Vector* vector) {
	if(vector == NULL) {
		return(NULL);
	}
	return vector->_data;
}

void* Vector_last(const Vector* vector) {
	if( vector == NULL ) {
		return(NULL);
	}
	return(vector->_data + ( (vector->_size - 1) * vector->_elementSize) );
}

int Vector_append(Vector* vector, const void* data) {
	if(vector == NULL || data == NULL) {
		return(VECTOR_ERR_NULL_ARG);
	}
	if(vector->_capacity == vector->_size) {
		Vector_resizeCapacity(vector, (vector->_size + 1) * CAPACITY_FACTOR);
	}
	memcpy(vector->_data + (vector->_size * vector->_elementSize ), data, vector->_elementSize);
	vector->_size++;

	return(VECTOR_FUNC_SUCCESS);
}

int Vector_chop(Vector* vector) {
	if(vector == NULL) {
		return(VECTOR_ERR_NULL_ARG);
	}
	if( vector->_size == 0 ) {
		return(VECTOR_EMPTY);
	}
	if(vector->_elementDestructor != NULL) {
		vector->_elementDestructor(vector->_data + ( (vector->_size - 1) * vector->_elementSize) );
	}
	vector->_size--;

	memset(vector->_data + (vector->_size * vector->_elementSize), '\0', vector->_elementSize);

	return(VECTOR_FUNC_SUCCESS);
}

int Vector_insert(Vector* vector, const void* data, long index) {
	if(vector == NULL || data == NULL) {
		return (VECTOR_ERR_NULL_ARG);
	}
	if(index < 0 || index >= vector->_size) {
		return(VECTOR_ERR_OUT_OF_BOUNDS);
	}
	if(vector->_capacity == vector->_size) {
		Vector_resizeCapacity(vector, (vector->_size + 1) * CAPACITY_FACTOR);
	}
	memmove(vector->_data + ( (index + 1) * vector->_elementSize),\
	        vector->_data + (index        * vector->_elementSize),\
	        (vector->_size - index) * vector->_elementSize);
	memcpy(vector->_data + (index * vector->_elementSize), data, vector->_elementSize);

	vector->_size++;

	return(VECTOR_FUNC_SUCCESS);
}

int Vector_remove(Vector* vector, long index) {
	if(vector == NULL) {
		return (VECTOR_ERR_NULL_ARG);
	}
	if(index < 0 || index >= vector->_size) {
		return(VECTOR_ERR_OUT_OF_BOUNDS);
	}
	if(vector->_elementDestructor != NULL) {
		vector->_elementDestructor(vector->_data + (index * vector->_elementSize) );
	}
	memmove(vector->_data + (index        * vector->_elementSize),\
	        vector->_data + ( (index + 1) * vector->_elementSize),\
	        (vector->_size - (index + 1) ) * vector->_elementSize);
	vector->_size--;

	memset(vector->_data + (vector->_size * vector->_elementSize), '\0', vector->_elementSize);
	
	return(VECTOR_FUNC_SUCCESS);
}