// Black's code at ov009 0x0217ea58-0x0217eb00. GameSpy's allocator wrappers (gsMemory's role; name guessed): default
// malloc/free/realloc/memalign and the wrappers that call the callbacks set at 0x0217ea9c. CheckRcode at
// 0x0217eb00 is gsPlatformSocket.c's.

#include "types.h"
#include "stddef.h"
#include "stdlib.h"
#include "system/aeabi.h"

typedef void *(*gsMallocCB)(size_t size);
typedef void (*gsFreeCB)(void *ptr);
typedef void *(*gsReallocCB)(void *ptr, size_t size);
typedef void *(*gsMemalignCB)(size_t align, size_t size);

void *func_ov009_0217ea58(size_t size);
void func_ov009_0217ea64(void *ptr);
void *func_ov009_0217ea70(void *ptr, size_t size);
void *func_ov009_0217ea7c(size_t align, size_t size);

// The functions that the allocations go through
struct {
    gsMallocCB malloc;
    gsFreeCB free;
    gsReallocCB realloc;
    gsMemalignCB memalign;
} data_ov009_021a3730 = { func_ov009_0217ea58, func_ov009_0217ea64, func_ov009_0217ea70, func_ov009_0217ea7c };

#pragma thumb off

void *func_ov009_0217ea58(size_t size) {
    return malloc(size);
}

void func_ov009_0217ea64(void *ptr) {
    free(ptr);
}

void *func_ov009_0217ea70(void *ptr, size_t size) {
    return realloc(ptr, size);
}



void *func_ov009_0217ea7c(size_t param_1, size_t param_2) {
    return calloc((size_t)__aeabi_uidivmod(param_2, param_1), param_1);
}
#pragma thumb on

#pragma thumb off
void func_ov009_0217ea9c(gsMallocCB malloc, gsFreeCB free, gsReallocCB realloc, gsMemalignCB memalign) {
    data_ov009_021a3730.malloc = malloc;
    data_ov009_021a3730.free = free;
    data_ov009_021a3730.realloc = realloc;
    data_ov009_021a3730.memalign = memalign;
}

void *gsimalloc(u32 size) {
    return data_ov009_021a3730.malloc(size);
}

void *gsirealloc(void *ptr, u32 size) {
    return data_ov009_021a3730.realloc(ptr, size);
}

void gsifree(void *ptr) {
    if (ptr != NULL) {
        data_ov009_021a3730.free(ptr);
    }
}

#pragma thumb on
