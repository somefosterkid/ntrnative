#include "global.h"

#include "sys_task.h"

void Heap_Free(void *ptr);

void sub_02030938(void *ptr);

void sub_02030940(void *arg0);

void sub_02030938(void *ptr) {
    Heap_Free(ptr);
}

void sub_02030940(void *dest) {
    MI_CpuFill8(dest, 0U, 0x58U);
}
