// MSL's memory allocation for NITRO: malloc and its kin on the main arena's current heap of NitroSDK. The file's name
// is a guess (MSL's alloc.c has malloc), its functions' pokebw2's.
#include <nitro/os.h>
#include <mem.h>
#include <stdlib.h>

#define CURRENT_HEAP (-1) // NitroSDK's OS_CURRENT_HEAP_HANDLE: the arena's current heap

void *__sys_alloc(size_t size) {
    return OS_AllocFromHeap(OS_ARENA_MAIN, CURRENT_HEAP, size);
}

void __sys_free(void *ptr) {
    OS_FreeToHeap(OS_ARENA_MAIN, CURRENT_HEAP, ptr);
}

size_t __sys_pointer_size(void *ptr) {
    return OS_ReferentSize(OS_ARENA_MAIN, ptr);
}

void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    return __sys_alloc(size);
}

void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    __sys_free(ptr);
}

void *realloc(void *ptr, size_t size) {
    void *newPtr;
    size_t oldSize;

    if (ptr == NULL) {
        return malloc(size);
    }
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    oldSize = __sys_pointer_size(ptr);
    newPtr  = malloc(size);
    if (newPtr != NULL) {
        if (oldSize >= size) {
            oldSize = size;
        }
        memcpy(newPtr, ptr, oldSize);
        free(ptr);
    }
    return newPtr;
}

void *calloc(size_t count, size_t size) {
    void *ptr;

    size *= count;
    ptr = malloc(size);
    if (ptr != NULL) {
        memset(ptr, 0, size);
    }
    return ptr;
}
