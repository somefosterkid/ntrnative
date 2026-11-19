#include "global.h"

#include "heap.h"
#include "sys_task.h"
#include "overlay_14_box_funcs.h"

void ov14_021E7468(UnkStruct_ov14_mon *mon) {
    Heap_Free(mon);
}
