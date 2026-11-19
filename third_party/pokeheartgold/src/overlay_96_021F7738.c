#include "global.h"

#include "sys_task.h"

#include "overlay_96_unk_struct_ov96_021_f7684.h"

void Heap_Free(void *ptr);

void ov96_021F7738(UnkStruct_ov96_021F7684 *ptr);

void ov96_021F7738(UnkStruct_ov96_021F7684 *ptr) {
    Heap_Free(ptr);
}
