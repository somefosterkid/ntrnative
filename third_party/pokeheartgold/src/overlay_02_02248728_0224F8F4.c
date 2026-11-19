#include "global.h"

#include "sys_task.h"

void Heap_Free(void *ptr);

void ov02_0224F8F4(void *ptr);

void ov02_0224F8F4(void *ptr) {
    Heap_Free(ptr);
}
