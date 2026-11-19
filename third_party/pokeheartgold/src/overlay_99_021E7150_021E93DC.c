#include "global.h"

#include "heap.h"
#include "pm_string.h"
#include "overlay_99_pokeathlon_event_record.h"

typedef struct UnkStruct_ov99_021E93DC_Heap_Alloc {
    s32 *unk_0;
    u8 unk_4;
    u8 unk_5[0x1B7];
    String *unk_1BC;
} UnkStruct_ov99_021E93DC_Heap_Alloc;

s32 ov99_021E92EC(void *, s32, void *);

void ov99_021E93DC(void *arg0, s32 *arg1);

void ov99_021E93DC(void *arg0, s32 *arg1) {
    UnkStruct_ov99_021E93DC_Heap_Alloc *dest;

    dest = Heap_Alloc(*((enum HeapID *)&((PokeathlonEventRecord *)arg0)->heapId), sizeof(UnkStruct_ov99_021E93DC_Heap_Alloc));
    MI_CpuFill8(dest, 0U, sizeof(UnkStruct_ov99_021E93DC_Heap_Alloc));
    dest->unk_0 = arg1;
    ov99_021E92EC(arg0, *arg1, (void *)&dest->unk_4);
    dest->unk_1BC = String_New(8U, *((enum HeapID *)&((PokeathlonEventRecord *)arg0)->heapId));
    *(void **)arg0 = dest;
}

void ov99_021E9418(void *arg0);

void ov99_021E9418(void *ptr) {
    String_Delete(*(String **)((u8 *)ptr + 0x1BC));
    Heap_Free(ptr);
}
