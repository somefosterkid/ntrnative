#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_96_unk_struct_ov96_021_f74a4.h"

UnkStruct_ov96_021F74A4 *ov96_021F74A4(enum HeapID param_1);

UnkStruct_ov96_021F74A4 *ov96_021F74A4(enum HeapID param_1) {
    UnkStruct_ov96_021F74A4 *dest;

    dest = Heap_Alloc(param_1, sizeof(UnkStruct_ov96_021F74A4));
    MI_CpuFill8(dest, 0, sizeof(UnkStruct_ov96_021F74A4));
    dest->timer = 0;
    return dest;
}
