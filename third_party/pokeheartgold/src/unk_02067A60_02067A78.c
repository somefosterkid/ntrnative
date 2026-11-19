#include "global.h"

#include "sys_task.h"

void Heap_Free(void *ptr);

void sub_02067A78(void *ptr);

void sub_02067A78(void *ptr) {
    Heap_Free(ptr);
}
