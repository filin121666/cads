#include <stdio.h>
#include "cads.h"

// Helper function to print dynamic array contents
void printDynamicArray(DynamicArray_t* array) {
    if (isEmptyDynamicArray(array)) {
        printf("Dynamic Array is empty.\n");
        return;
    }

    printf("Dynamic Array contents:\n");
    for (size_t i = 0; i < array->size; ++i) {
        switch (array->type) {
            case INT8:
                printf("%d ", *((int8_t*)getElementByIndexDynamicArray(array, i)));
                break;
            case INT16:
                printf("%d ", *((int16_t*)getElementByIndexDynamicArray(array, i)));
                break;
            case INT32:
                printf("%d ", *((int32_t*)getElementByIndexDynamicArray(array, i)));
                break;
            case INT64:
                printf("%ld ", *((int64_t*)getElementByIndexDynamicArray(array, i)));
                break;
            case UINT8:
                printf("%u ", *((uint8_t*)getElementByIndexDynamicArray(array, i)));
                break;
            case UINT16:
                printf("%u ", *((uint16_t*)getElementByIndexDynamicArray(array, i)));
                break;
            case UINT32:
                printf("%u ", *((uint32_t*)getElementByIndexDynamicArray(array, i)));
                break;
            case UINT64:
                printf("%lu ", *((uint64_t*)getElementByIndexDynamicArray(array, i)));
                break;
            case FLOAT:
                printf("%f ", *((float*)getElementByIndexDynamicArray(array, i)));
                break;
            case DOUBLE:
                printf("%lf ", *((double*)getElementByIndexDynamicArray(array, i)));
                break;
            case LDOUBLE:
                printf("%Lf ", *((long double*)getElementByIndexDynamicArray(array, i)));
                break;
            case CHAR:
                printf("%c ", *((char*)getElementByIndexDynamicArray(array, i)));
                break;
            case BOOL:
                printf("%s ", (*((bool*)getElementByIndexDynamicArray(array, i))) ? "true" : "false");
                break;
            default:
                printf("Unknown type.\n");
        }
    }
    printf("\n");
}

int main() {
    // Create a dynamic array of integers
    DynamicArray_t* intArray = createDynamicArray(INT32, 0, 5);

    // Push elements to the back
    int elements[] = {10, 20, 30, 40, 50};
    for (size_t i = 0; i < sizeof(elements) / sizeof(elements[0]); ++i) {
        pushBackDynamicArray(intArray, &elements[i]);
    }

    printf("After pushing elements to the back:\n");
    printDynamicArray(intArray);

    // Push elements to the front
    int newElement = 5;
    pushFrontDynamicArray(intArray, &newElement);

    printf("After pushing an element to the front:\n");
    printDynamicArray(intArray);

    // Insert an element at index 2
    int insertedElement = 15;
    insertElementDynamicArray(intArray, 2, &insertedElement);

    printf("After inserting an element at index 2:\n");
    printDynamicArray(intArray);

    // Remove an element from the back
    popBackDynamicArray(intArray);

    printf("After removing an element from the back:\n");
    printDynamicArray(intArray);

    // Remove an element from the front
    popFrontDynamicArray(intArray);

    printf("After removing an element from the front:\n");
    printDynamicArray(intArray);

    // Access elements by index and update
    int updateValue = 25;
    setElementDynamicArray(intArray, 1, &updateValue);

    printf("After updating element at index 1:\n");
    printDynamicArray(intArray);

    // Free memory
    freeDynamicArray(intArray);

    return 0;
}