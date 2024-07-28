#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "cads.h"


static size_t getTypeSize(DataType_t type) {
    switch (type)
    {
    case INT8: return sizeof(int8_t);
    case INT16: return sizeof(int16_t);
    case INT32: return sizeof(int32_t);
    case INT64: return sizeof(int64_t);
    case UINT8: return sizeof(uint8_t);
    case UINT16: return sizeof(uint16_t);
    case UINT32: return sizeof(uint32_t);
    case UINT64: return sizeof(uint64_t);
    case FLOAT: return sizeof(float);
    case DOUBLE: return sizeof(double);
    case LDOUBLE: return sizeof(long double);
    case CHAR: return sizeof(char);
    case BOOL: return sizeof(bool);
    default: return 0;
    }
}

// Create dynamic array
DynamicArray_t* createDynamicArray(DataType_t type, size_t init_size, size_t init_cap) {
    DynamicArray_t* array = (DynamicArray_t*) malloc(sizeof(DynamicArray_t));
    if (array) {
        array->type = type;
        array->type_size = getTypeSize(type);
        array->size = init_size;
        array->capacity = init_cap;
        array->data = malloc((init_size + init_cap) * array->type_size);
        if (!array->data) {
            free(array);
            return NULL;
        }
    }
    return array;
}

// Free dynamic array
void freeDynamicArray(DynamicArray_t* array) {
    if (array) {
        if (array->data) {
            free(array->data);
        }
        free(array);
    }
}

// Initializes the array with zeros
void initZerosDynamicArray(DynamicArray_t* array) {
    if (array && array->data) {
        memset(array->data, 0, (array->size + array->capacity) * array->type_size);
    }
}

// Double the capacity dynamic array
void expandDynamicArray(DynamicArray_t* array) {
    array->capacity *= 2;
    array->data = realloc(array->data, (array->size + array->capacity) * array->type_size);
    if (!array->data) {
        array->capacity /= 2;
    }
}

// Increase the capacity by value dynamic array
void expandDynamicArrayWithSize(DynamicArray_t* array, size_t value) {
    array->capacity += value;
    array->data = realloc(array->data, (array->size + array->capacity) * array->type_size);
    if (!array->data) {
        array->capacity -= value;
    }
}

// Insert a value at the end of a dynamic array
void pushBackDynamicArray(DynamicArray_t* array, void* value) {
    if (array->size >= array->capacity) expandDynamicArray(array);

    void* target = (char*) array->data + array->size * array->type_size;
    memcpy(target, value, array->type_size);
    array->size++;
}

// Delete the last element of the dynamic array
void popBackDynamicArray(DynamicArray_t* array) {
    if (array->size == 0) return;

    array->size--;
}

// Insert a value at the beginning of a dynamic array
void pushFrontDynamicArray(DynamicArray_t* array, void* value) {
    if (array->size >= array->capacity) expandDynamicArray(array);

    char* data = (char*) array->data;
    memmove(data + array->type_size, data, array->size * array->type_size);
    memcpy(data, value, array->type_size);
    array->size++;
}

// Remove a value from the beginning of a dynamic array
void popFrontDynamicArray(DynamicArray_t* array) {
    if (array->size == 0) return;

    char* data = (char*) array->data;
    memmove(data, data + array->type_size, (array->size - 1) * array->type_size);
    array->size--;
}

// Checks if the array is empty
bool isEmptyDynamicArray(DynamicArray_t* array) {
    return array->size == 0;
}

// Returns a pointer to an element by index (you need to convert it to a pointer of the desired type using type casting)
void* getElementByIndexDynamicArray(DynamicArray_t* array, size_t index) {
    if (index >= array->size) return NULL;

    char* data = (char*) array->data;
    return data + (index * array->type_size);
}

// Returns a pointer to the first element (you need to convert it to a pointer of the desired type using type casting)
void* getBeginElementDynamicArray(DynamicArray_t* array) {
    if (array->size == 0) return NULL;

    return array->data;
}

// Returns a pointer to the last element (you need to convert it to a pointer of the desired type using type casting)
void* getEndElementDynamicArray(DynamicArray_t* array) {
    if (array->size == 0) return NULL;

    char* data = (char*) array->data;
    return data + ((array->size - 1) * array->type_size);
}

// Changes the value of an element of a dynamic array by the index index to value
void setElementDynamicArray(DynamicArray_t* array, size_t index, void* value) {
    if (index >= array->size) return;

    char* data = (char*) array->data;
    void* target = data + (index * array->type_size);
    memcpy(target, value, array->type_size);
}

// Inserting an element into an array by index
void insertElementDynamicArray(DynamicArray_t* array, size_t index, void* value) {
    if (index >= array->size) return;
    if (array->size >= array->capacity) expandDynamicArray(array);

    char* data = (char*) array->data;
    memmove(data + (index + 1) * array->type_size, data + index * array->type_size, (array->size - index) * array->type_size);
    memcpy(data + index * array->type_size, value, array->type_size);
    array->size++;
}

// Remove an element from a dynamic array by index
void removeElementDynamicArray(DynamicArray_t* array, size_t index) {
    if (index >= array->size) return;
    if (array->size == 0) return;

    char* data = (char*) array->data;
    memmove(data + index * array->type_size, data + (index + 1) * array->type_size, (array->size - index - 1) * array->type_size);
    array->size--;
}
