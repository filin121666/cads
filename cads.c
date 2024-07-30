#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>
#include <string.h>

#include "cads.h"

#define ROTLEFT(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
#define ROTRIGHT(a, b) (((a) >> (b)) | ((a) << (32 - (b))))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))
#define SIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))
// typedef unsigned char BYTE;
// typedef unsigned int  WORD;

typedef struct
{
    unsigned char data[64];
    unsigned int datalen;
    unsigned long long bitlen;
    unsigned int state[8];
} SHA256_CTX;

static const unsigned int k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

static void sha256_transform(SHA256_CTX *ctx, const unsigned char data[])
{
    unsigned int a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    for (; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];

    for (i = 0; i < 64; ++i)
    {
        t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

static void sha256_init(SHA256_CTX *ctx)
{
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

static void sha256_update(SHA256_CTX *ctx, const unsigned char data[], size_t len)
{
    unsigned int i;

    for (i = 0; i < len; ++i)
    {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64)
        {
            sha256_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

static void sha256_final(SHA256_CTX *ctx, unsigned char hash[])
{
    unsigned int i;

    i = ctx->datalen;

    // Pad whatever data is left in the buffer.
    if (ctx->datalen < 56)
    {
        ctx->data[i++] = 0x80;
        while (i < 56)
            ctx->data[i++] = 0x00;
    }
    else
    {
        ctx->data[i++] = 0x80;
        while (i < 64)
            ctx->data[i++] = 0x00;
        sha256_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }

    // Append to the padding the total message's length in bits and transform.
    ctx->bitlen += ctx->datalen * 8;
    ctx->data[63] = ctx->bitlen;
    ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16;
    ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32;
    ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48;
    ctx->data[56] = ctx->bitlen >> 56;
    sha256_transform(ctx, ctx->data);

    // Since this implementation uses little endian byte ordering and SHA uses big endian,
    // reverse all the bytes when copying the final state to the output hash.
    for (i = 0; i < 4; ++i)
    {
        hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
}

static size_t getTypeSize(DataType_t type)
{
    switch (type)
    {
    case INT8:
        return sizeof(int8_t);
    case INT16:
        return sizeof(int16_t);
    case INT32:
        return sizeof(int32_t);
    case INT64:
        return sizeof(int64_t);
    case UINT8:
        return sizeof(uint8_t);
    case UINT16:
        return sizeof(uint16_t);
    case UINT32:
        return sizeof(uint32_t);
    case UINT64:
        return sizeof(uint64_t);
    case FLOAT:
        return sizeof(float);
    case DOUBLE:
        return sizeof(double);
    case LDOUBLE:
        return sizeof(long double);
    case CHAR:
        return sizeof(char);
    case BOOL:
        return sizeof(bool);
    default:
        return 0;
    }
}

// Dynamic array implementation in c

// Create dynamic array
DynamicArray_t *createDynamicArray(DataType_t type, size_t init_size, size_t init_cap)
{
    DynamicArray_t *array = (DynamicArray_t *)malloc(sizeof(DynamicArray_t));
    if (array)
    {
        array->type = type;
        array->type_size = getTypeSize(type);
        array->size = init_size;
        array->capacity = init_cap;
        array->data = malloc((init_size + init_cap) * array->type_size);
        if (!array->data)
        {
            free(array);
            return NULL;
        }
    }
    return array;
}

// Free dynamic array
void freeDynamicArray(DynamicArray_t *array)
{
    if (array)
    {
        if (array->data)
        {
            free(array->data);
        }
        free(array);
    }
}

// Initializes the array with zeros
void initZerosDynamicArray(DynamicArray_t *array)
{
    if (array && array->data)
    {
        memset(array->data, 0, (array->size + array->capacity) * array->type_size);
    }
}

// Double the capacity dynamic array
void expandDynamicArray(DynamicArray_t *array)
{
    array->capacity *= 2;
    array->data = realloc(array->data, (array->size + array->capacity) * array->type_size);
    if (!array->data)
    {
        array->capacity /= 2;
    }
}

// Increase the capacity by value dynamic array
void expandDynamicArrayWithSize(DynamicArray_t *array, size_t value)
{
    array->capacity += value;
    array->data = realloc(array->data, (array->size + array->capacity) * array->type_size);
    if (!array->data)
    {
        array->capacity -= value;
    }
}

// Insert a value at the end of a dynamic array
void pushBackDynamicArray(DynamicArray_t *array, void *value)
{
    if (array->size >= array->capacity)
        expandDynamicArray(array);

    void *target = (char *)array->data + array->size * array->type_size;
    memcpy(target, value, array->type_size);
    array->size++;
}

// Delete the last element of the dynamic array
void popBackDynamicArray(DynamicArray_t *array)
{
    if (array->size == 0)
        return;

    array->size--;
}

// Insert a value at the beginning of a dynamic array
void pushFrontDynamicArray(DynamicArray_t *array, void *value)
{
    if (array->size >= array->capacity)
        expandDynamicArray(array);

    char *data = (char *)array->data;
    memmove(data + array->type_size, data, array->size * array->type_size);
    memcpy(data, value, array->type_size);
    array->size++;
}

// Remove a value from the beginning of a dynamic array
void popFrontDynamicArray(DynamicArray_t *array)
{
    if (array->size == 0)
        return;

    char *data = (char *)array->data;
    memmove(data, data + array->type_size, (array->size - 1) * array->type_size);
    array->size--;
}

// Checks if the array is empty
bool isEmptyDynamicArray(DynamicArray_t *array)
{
    return array->size == 0;
}

// Returns a pointer to an element by index (you need to convert it to a pointer of the desired type using type casting)
void *getElementByIndexDynamicArray(DynamicArray_t *array, size_t index)
{
    if (index >= array->size)
        return NULL;

    char *data = (char *)array->data;
    return data + (index * array->type_size);
}

// Returns a pointer to the first element (you need to convert it to a pointer of the desired type using type casting)
void *getBeginElementDynamicArray(DynamicArray_t *array)
{
    if (array->size == 0)
        return NULL;

    return array->data;
}

// Returns a pointer to the last element (you need to convert it to a pointer of the desired type using type casting)
void *getEndElementDynamicArray(DynamicArray_t *array)
{
    if (array->size == 0)
        return NULL;

    char *data = (char *)array->data;
    return data + ((array->size - 1) * array->type_size);
}

// Changes the value of an element of a dynamic array by the index index to value
void setElementDynamicArray(DynamicArray_t *array, size_t index, void *value)
{
    if (index >= array->size)
        return;

    char *data = (char *)array->data;
    void *target = data + (index * array->type_size);
    memcpy(target, value, array->type_size);
}

// Inserting an element into an array by index
void insertElementDynamicArray(DynamicArray_t *array, size_t index, void *value)
{
    if (index >= array->size)
        return;
    if (array->size >= array->capacity)
        expandDynamicArray(array);

    char *data = (char *)array->data;
    memmove(data + (index + 1) * array->type_size, data + index * array->type_size, (array->size - index) * array->type_size);
    memcpy(data + index * array->type_size, value, array->type_size);
    array->size++;
}

// Remove an element from a dynamic array by index
void removeElementDynamicArray(DynamicArray_t *array, size_t index)
{
    if (index >= array->size)
        return;
    if (array->size == 0)
        return;

    char *data = (char *)array->data;
    memmove(data + index * array->type_size, data + (index + 1) * array->type_size, (array->size - index - 1) * array->type_size);
    array->size--;
}

// SHA-256 implementation in C

// Return SHA-256 hash code generated from data
unsigned char *sha256(unsigned char *data)
{
    SHA256_CTX ctx;
    unsigned char *result = (unsigned char *) malloc(SHA256_BLOCK_SIZE * sizeof(unsigned char));
    if (!result) {
        return NULL;
    }
    sha256_init(&ctx);
    sha256_update(&ctx, data, strlen((char *)data));
    sha256_final(&ctx, result);
    return (unsigned char *)result;
}