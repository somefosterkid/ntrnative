#include "global.h"

#include "sys_task.h"

void Heap_Free(void *ptr);

void sub_020314BC(void *ptr);

void sub_020314BC(void *ptr) {
    Heap_Free(ptr);
}
