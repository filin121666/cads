#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define SHA256_BLOCK_SIZE 32

// data structures

typedef enum
{
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    FLOAT,
    DOUBLE,
    LDOUBLE, // long double
    CHAR,
    BOOL,
    // DYNAMICARRAY  // dynamic array
} DataType_t;

// Dynamic array
typedef struct
{
    DataType_t type;
    size_t type_size;
    size_t size;
    size_t capacity;
    void *data;
} DynamicArray_t;

DynamicArray_t *createDynamicArray(DataType_t type, size_t init_size, size_t init_cap);
void freeDynamicArray(DynamicArray_t *array);
void initZerosDynamicArray(DynamicArray_t *array);
void expandDynamicArray(DynamicArray_t *array);
void expandDynamicArrayWithSize(DynamicArray_t *array, size_t value);
void pushBackDynamicArray(DynamicArray_t *array, void *value);
void popBackDynamicArray(DynamicArray_t *array);
void pushFrontDynamicArray(DynamicArray_t *array, void *value);
void popFrontDynamicArray(DynamicArray_t *array);
bool isEmptyDynamicArray(DynamicArray_t *array);
void *getElementByIndexDynamicArray(DynamicArray_t *array, size_t index);
void *getBeginElementDynamicArray(DynamicArray_t *array);
void *getEndElementDynamicArray(DynamicArray_t *array);
void setElementDynamicArray(DynamicArray_t *array, size_t index, void *value);
void insertElementDynamicArray(DynamicArray_t *array, size_t index, void *value);
void removeElementDynamicArray(DynamicArray_t *array, size_t index);

// algorithms
unsigned char *sha256(unsigned char *data);