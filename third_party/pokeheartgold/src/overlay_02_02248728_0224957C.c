#include "global.h"

#include "sys_task.h"

void Heap_Free(void *ptr);

void ov02_0224957C(void *ptr);

void ov02_0224957C(void *ptr) {
    Heap_Free(ptr);
}
