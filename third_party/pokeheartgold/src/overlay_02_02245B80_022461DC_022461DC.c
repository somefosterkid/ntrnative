#include "global.h"

#include "field/overlay_01_021FB878.h"

#include "field_types_def.h"
#include "field_system_def.h"
#include "heap.h"
#include "overlay_02_02245B80.h"
#include "player_avatar.h"
#include "script_manager.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "task.h"
#include "unk_02005D10.h"

void ov02_022462DC(SysTask *task, void *windParticleAnimEnv);

extern u32 ov02_02253254;

extern u32 ov02_02253D80;

extern u32 ov02_02253D84;

BOOL ov02_022461DC(TaskManager *param_1);

BOOL ov02_022461DC(TaskManager *param_1) {
    LegendWingTask *ptr;
    s32 iVar3;
    VecFx32 VStack_18;

    ptr = TaskManager_GetEnvironment(param_1);
    switch (ptr->state) {
    case 0:
        ptr->effect = ov02_02245E04((const Ov02Field3dEffectDesc *)(&ov02_02253254 + (u32)ptr->wingIdx * 2), ptr->heapID);
        PlayerAvatar_CopyPositionVector(ptr->fieldSystem->playerAvatar, &VStack_18);
        Field3dObject_SetPosEx(&ptr->effect->object, VStack_18.x, VStack_18.y, VStack_18.z);
        PlaySE(0x90f);
        ptr->state += 1;
        break;
    case 1:
        iVar3 = ov02_02245FD0(ptr->effect);
        if (iVar3 != 0) {
            ov02_02245ED8(ptr->effect, (NarcId)0xae, *(u16 **)(&ov02_02253D80 + (u32)ptr->wingIdx * 2), 3);
            ptr->sysTask = SysTask_CreateOnMainQueue(ov02_022462DC, ptr, 0);
            QueueScript(param_1, 3, (LocalMapObject *)0x0, (void *)0x0);
            ptr->state += 1;
        }
        break;
    case 2:
        SysTask_Destroy(ptr->sysTask);
        ptr->state += 1;
    case 3:
        iVar3 = ov02_02245FD0(ptr->effect);
        if (iVar3 != 0) {
            ov02_02245ED8(ptr->effect, (NarcId)0xae, *(u16 **)(&ov02_02253D84 + (u32)ptr->wingIdx * 2), 3);
            ptr->state += 1;
code_r0x020000c8:
            iVar3 = ov02_02245FD0(ptr->effect);
            if (iVar3 != 0) {
                ov02_02245E68(ptr->effect);
                Heap_Free(ptr);
                return 1;
            }
        }
        break;
    case 4:
        goto code_r0x020000c8;
    }
    return 0;
}
