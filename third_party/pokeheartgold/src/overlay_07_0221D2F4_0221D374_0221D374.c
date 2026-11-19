#include "global.h"

#include "brightness.h"
#include "heap.h"
#include "sys_task_api.h"
#include "overlay_07_battle_anim_system.h"

s32 ov07_0221D2F4(void *, s32);

void ov07_0221D330(SysTask *task, void *data);

typedef struct UnkStruct_ov07_0221D374 {
    void *ctx;
    u32 unk4_0 : 8;
    u32 unk4_8 : 24;
} UnkStruct_ov07_0221D374;

void ov07_0221D374(BattleAnimSystem *arg0, s32 arg1, s32 arg2);

void ov07_0221D374(BattleAnimSystem *arg0, s32 arg1, s32 arg2) {
    UnkStruct_ov07_0221D374 *work = Heap_Alloc(*(enum HeapID *)arg0, sizeof(UnkStruct_ov07_0221D374));
    u32 **script;

    MI_CpuFill8(work, 0, sizeof(UnkStruct_ov07_0221D374));
    work->ctx = arg0;
    script = &arg0->scriptPtr;
    *script += 1;
    work->unk4_0 = (u8) * *script;
    *script += 1;
    ScreenBrightnessData_InitAll();
    ov07_0221D2F4(work, 1);
    SysTask_CreateOnMainQueue(ov07_0221D330, work, 0x1001);
}
