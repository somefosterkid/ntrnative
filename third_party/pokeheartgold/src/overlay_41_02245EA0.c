#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "error_handling.h"
#include "touchscreen.h"
#include "overlay_41_unk_struct_ov22_0225500c.h"
#include "overlay_41_unk_struct_ov22_02254de0.h"

void *ov41_0224607C(void *);
void *ov41_022460A8(void *);
void *ov41_02245EE0(void *arg0);
s32 ov41_022460DC(void *, TouchscreenHitbox *);
void ov41_02245F04(void *arg0);

UnkStruct_ov22_02254DE0 *ov41_02245EA0(s32 count, enum HeapID heapID) {
    UnkStruct_ov22_02254DE0 *list;
    UnkStruct_ov22_02255040 *items;

    list = Heap_Alloc(heapID, sizeof(UnkStruct_ov22_02254DE0));
    items = Heap_Alloc(heapID, count * sizeof(UnkStruct_ov22_02255040));
    list->unk_00 = items;
    memset(items, 0, count * sizeof(UnkStruct_ov22_02255040));
    list->unk_04 = count;
    return list;
}

void ov41_02245ECC(UnkStruct_ov22_02254DE0 *list) {
    Heap_Free(list->unk_00);
    Heap_Free(list);
}

void *ov41_02245EE0(void *arg0) {
    void *v0;

    v0 = ov41_0224607C(*(void **)arg0);
    if (v0 == NULL) {
        GF_AssertFail();
    }
    *(s32 *)v0 = (s32) *((s32 *)&((UnkStruct_ov22_0225500C *)arg0)->unk_18);
    *(void **)((u8 *)v0 + 4) = ov41_022460A8(arg0);
    return v0;
}

void ov41_02245F04(void *arg0) {
    TouchscreenHitbox hitbox;

    ov41_022460DC(arg0, &hitbox);
    TouchscreenHitbox_TouchHeldIsIn(&hitbox);
}
