#include "global.h"

#include "constants/heap.h"

#include "overlay_49_unk_struct_ov70_0225_dee8_t.h"

typedef struct UnkStruct_ov70_0225DEE8_t UnkStruct_ov70_0225DEE8;

void ov49_0225AEA8(UnkStruct_ov70_0225EC20 *, u32, enum HeapID, u32);

void ov49_0225A10C(UnkStruct_ov70_0225DEE8 *arg0, u32 arg1);

void ov49_0225A120(UnkStruct_ov70_0225DEE8 *arg0, u32 arg1, u32 arg2);

void ov49_0225A10C(UnkStruct_ov70_0225DEE8 *arg0, u32 arg1) {
    ov49_0225AEA8(&arg0->unk_39C, arg1, HEAP_ID_120, 0);
}

void ov49_0225A120(UnkStruct_ov70_0225DEE8 *arg0, u32 arg1, u32 arg2) {
    ov49_0225AEA8(&arg0->unk_39C, arg1, HEAP_ID_120, arg2);
}
