#include "global.h"
struct UnkStruct_ov22_02257F50;

#include "error_handling.h"
#include "heap.h"
#include "sys_task.h"
#include "systask_environment.h"
#include "overlay_41_unk_struct_ov22_02257_f50.h"

s32 ov41_02248AE0(void *, s32, s32);

void ov41_02248BFC(SysTask *task, void *data);

void ov41_02248B84(struct UnkStruct_ov22_02257F50 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void ov41_02248B84(UnkStruct_ov22_02257F50 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 v0;
    s32 v2;
    s32 v4;
    void *v1;
    void *v3;

    v0 = arg3;
    v1 = SysTask_GetData(CreateSysTaskAndEnvironment(ov41_02248BFC, 0x30U, 0U, HEAP_ID_13));
    *(void **)v1 = arg0;
    *(s32 *)((u8 *)v1 + 4) = arg1;
    *(s32 *)((u8 *)v1 + 8) = arg2;
    *(s32 *)((u8 *)v1 + 0xC) = v0;
    *(s32 *)((u8 *)v1 + 0x10) = arg4;
    *(s32 *)((u8 *)v1 + 0x1C) = 0;
    *(s32 *)((u8 *)v1 + 0x20) = ov41_02248AE0(arg0, arg1, arg2);
    v2 = ov41_02248AE0(arg0, v0, arg4);
    *(s32 *)((u8 *)v1 + 0x24) = v2;
    v4 = *(s32 *)((u8 *)v1 + 0x20) + v2;
    *(s32 *)((u8 *)v1 + 0x2C) = v4;
    v3 = Heap_Alloc(HEAP_ID_13, 0xC * v4);
    *(void **)((u8 *)v1 + 0x28) = v3;
    if (v3 == NULL) {
        GF_AssertFail();
    }
    memset(*(void **)((u8 *)v1 + 0x28), 0, 0xC * *(s32 *)((u8 *)v1 + 0x2C));
    *((s32 *)&arg0->unk_00.unk_3C) = 1;
}
