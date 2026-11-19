#include "global.h"

#include "heap.h"
#include "overlay_80_0223AC24.h"
#include "sys_task_api.h"

typedef struct UnkStruct_ov80_0223AC24_Heap_Alloc {
    u8 unk_0[0x4];
    s32 unk_4;
    u8 unk_8[0x4];
    s32 unk_C;
    s32 unk_10;
    s32 unk_14;
    u8 unk_18[0x30];
    s32 *unk_48;
} UnkStruct_ov80_0223AC24_Heap_Alloc;

void ov80_0223AC68(SysTask *task, void *data);

void ov80_0223AC24(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 arg4) {
    s32 v0;
    UnkStruct_ov80_0223AC24_Heap_Alloc *v1;

    v0 = arg2;
    v1 = Heap_Alloc(HEAP_ID_FIELD2, sizeof(UnkStruct_ov80_0223AC24_Heap_Alloc));
    memset(v1, 0, sizeof(UnkStruct_ov80_0223AC24_Heap_Alloc));
    SysTask_CreateOnMainQueue(ov80_0223AC68, v1, 5U);
    v1->unk_48 = arg3;
    if (arg3 != NULL) {
        *arg3 = 0;
    }
    v1->unk_C = arg0;
    v1->unk_10 = arg1;
    v1->unk_14 = v0;
    v1->unk_4 = arg4;
}
