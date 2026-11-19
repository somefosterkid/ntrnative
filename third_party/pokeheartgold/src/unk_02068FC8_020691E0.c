#include "global.h"

#include "sys_task.h"

void Heap_Free(void *ptr);

void sub_020691E0(void *ptr);

void sub_020691E0(void *ptr) {
    Heap_Free(ptr);
}
