#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "overlay_96_arena_cpu.h"

Ov96ArenaCpu *ov96_021F3390(enum HeapID heapId, u8 count, BOOL flag) {
    Ov96ArenaCpu *data;

    data = Heap_Alloc(heapId, sizeof(Ov96ArenaCpu));
    MI_CpuFill8(data, 0, sizeof(Ov96ArenaCpu));
    data->count = count;
    data->unk_4E9 = data->unk_4EA = 4 - count;
    if (flag == FALSE) {
        data->interval = 20;
    } else {
        data->interval = 10;
    }
    data->timer = data->interval;
    return data;
}
