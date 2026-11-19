#include "global.h"

#include "sys_task.h"

void Heap_Free(void *ptr);

void ov01_021F1448(void *ptr);

void ov01_021F1448(void *ptr) {
    Heap_Free(ptr);
}
