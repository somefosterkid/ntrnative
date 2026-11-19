#include "global.h"

#include "constants/heap.h"

#include "error_handling.h"
#include "overlay_42.h"
#include "heap.h"

struct UnkStruct_ov70_022610B8_t;

typedef struct UnkStruct_ov70_022610B8_t UnkStruct_ov70_022610B8;

void ov49_0225D1C4(UnkStruct_ov70_022610B8 *param0, UnkStruct_ov44_02232914 param1);

UnkStruct_ov44_02232914 ov49_0225D1EC(const UnkStruct_ov70_022610B8 *param0);

extern const s32 ov49_02269A74[2];

void ov49_0225EE4C(UnkStruct_ov70_022610B8 *param0, u32 param1);

s32 ov49_0225F068(void *, s32, enum HeapID);
void *ov49_0225EEAC(void *arg0, enum HeapID arg1);

void ov49_0225EE4C(UnkStruct_ov70_022610B8 *param0, u32 param1) {
    UnkStruct_ov44_02232914 v0;
    u32 v1;

    GF_ASSERT((param1 == 92) || (param1 == 93));

    v1 = param1 - 92;
    v0 = ov49_0225D1EC(param0);
    v0.unk0 += ov49_02269A74[v1];
    v0.unk2 += -10;

    ov49_0225D1C4(param0, v0);
}

void *ov49_0225EEAC(void *arg0, enum HeapID heapID) {
    s32 v1;
    void *v0;
    void *v2;

    v0 = Heap_Alloc(heapID, 0x2FCU);
    memset(v0, 0, 0x2FCU);
    *(enum HeapID *)v0 = heapID;
    *(void **)((u8 *)v0 + 4) = arg0;
    v1 = 0;
    v2 = v0 + 8;
    do {
        ov49_0225F068(v2, v1, heapID);
        v1 += 1;
        v2 += 0x24;
    } while (v1 < 0x14);
    ov49_0225F068(v0 + 0x2D8, 0, heapID);
    return v0;
}
