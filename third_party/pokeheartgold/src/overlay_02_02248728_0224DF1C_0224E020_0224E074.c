#include "global.h"

#include "constants/heap.h"

#include "field_types_def.h"
#include "heap.h"
#include "sys_task_api.h"
#include "sys_task.h"

void ov02_0224E020(SysTask *param0, void *param1);

// the task data ov02_0224E020 reads (overlay_02_02248728_0224E008.c's UnkStruct_ov6_02248948): the object task, the
// result var, the state and the case; two pointers, so 16 bytes on the DS and 24 on a 64-bit host
struct UnkStruct_ov5_021D1BEC_t;
typedef struct {
    struct UnkStruct_ov5_021D1BEC_t *unk_00;
    u16 *unk_04;
    u32 unk_08;
    u32 unk_0C;
} UnkStruct_ov6_02248948;

extern struct UnkStruct_ov5_021D1BEC_t *(*ov02_02253A1C[6])(FieldSystem *);

void ov02_0224E074(FieldSystem *fieldSystem, u16 *p_ret, s32 a2, enum HeapID heapID);

void ov02_0224E074(FieldSystem *fieldSystem, u16 *p_ret, s32 a2, enum HeapID heapID) {
    UnkStruct_ov6_02248948 *data;
    u32 iVar2;
    u8 *puVar3;

    data = Heap_Alloc(heapID, sizeof(UnkStruct_ov6_02248948));
    puVar3 = (u8 *)data;
    iVar2 = sizeof(UnkStruct_ov6_02248948);
    do {
        *puVar3 = 0;
        puVar3++;
        iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
    data->unk_00 = ov02_02253A1C[a2](fieldSystem);
    data->unk_04 = p_ret;
    data->unk_0C = a2;
    *p_ret = 0;
    SysTask_CreateOnMainQueue(ov02_0224E020, data, 0);
    return;
}
