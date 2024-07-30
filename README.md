# CADS (C Algorithms and Data Structures)
Is a library that implements structures and algorithms in c.
## Documentation
### DataType_t
- It is an enum type that consists of data types that use data structures from the library.
### Data structures
___
#### Dynamic array (DynamicArray_t)
- Consists of:
    - a type field of the DataType_t type;
    - a type_size field of the size_t type;
    - a size field of the size_t type;
    - a capacity field of type size_t;
    - a data field of type void* (a pointer to the data in the array).
- functions:
    - createDynamicArray
        ```c
        DynamicArray_t *createDynamicArray(DataType_t type, size_t init_size, size_t init_cap);
        ```
        Сreates a dynamic array and returns a pointer to it, if this fails, then a null pointer is returned (preferably checking for its existence).
    - freeDynamicArray
        ```c
        void freeDynamicArray(DynamicArray_t *array);
        ```
        Deletes the array and clears the memory behind it.
    - initZerosDynamicArray
        ```c
        void initZerosDynamicArray(DynamicArray_t *array);
        ```
        Initializes the array with zeros.
    - expandDynamicArray
        ```c
        void expandDynamicArray(DynamicArray_t *array);
        ```
        Doubles the capacity of the array.
    - expandDynamicArrayWithSize
        ```c
        void expandDynamicArrayWithSize(DynamicArray_t *array, size_t value);
        ```
        Increases the capacity of the array by a value.
    - pushBackDynamicArray
        ```c
        void pushBackDynamicArray(DynamicArray_t *array, void *value);
        ```
        Adds an element to the end of the array.
    - popBackDynamicArray
        ```c
        void popBackDynamicArray(DynamicArray_t *array);
        ```
        Deletes the last element of the array.
    - pushFrontDynamicArray
        ```c
        void pushFrontDynamicArray(DynamicArray_t *array, void *value);
        ```
        Adds an element to the beginning of the array.
    - popFrontDynamicArray
        ```c
        void popFrontDynamicArray(DynamicArray_t *array);
        ```
        Deletes the first element of the array and moves it to the left.
    - isEmptyDynamicArray
        ```c
        bool isEmptyDynamicArray(DynamicArray_t *array);
        ```
        Checks if the array is empty.
    - getElementByIndexDynamicArray
        ```c
        void *getElementByIndexDynamicArray(DynamicArray_t *array, size_t index);
        ```
        Returns a pointer to an element by index (you need to convert it to a pointer of the desired type using type casting).
    - getBeginElementDynamicArray
        ```c
        void *getBeginElementDynamicArray(DynamicArray_t *array);
        ```
        Returns a pointer to the first element (you need to convert it to a pointer of the desired type using type casting).
    - getEndElementDynamicArray
        ```c
        void *getEndElementDynamicArray(DynamicArray_t *array);
        ```
        Returns a pointer to the last element (you need to convert it to a pointer of the desired type using type casting).
    - insertElementDynamicArray
        ```c
        void insertElementDynamicArray(DynamicArray_t *array, size_t index, void *value);
        ```
        Inserting an element into an array by index.
    - removeElementDynamicArray
        ```c
        void removeElementDynamicArray(DynamicArray_t *array, size_t index);
        ```
        Remove an element from a dynamic array by index.
### Algorithms
___
#### SHA-256
```c
unsigned char *sha256(unsigned char *data);
```
Hashes the string using the SHA-256 algorithm and returns a pointer to the hashing result