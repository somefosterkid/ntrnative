#include "global.h"

#include "constants/heap.h"

#include "heap.h"
#include "network_icon.h"
#include "sys_task.h"
#include "sys_task_api.h"

void sub_0203A4D4(u32, u32, s32, enum HeapID);

void sub_0203A59C(u32, u32, enum HeapID);

void sub_0203A42C(SysTask *task, void *param1);

NetworkIcon *sub_0203A3B0(u32 param_1, enum HeapID param_2, u16 param_3, u16 param_4, u32 param_5, void *param_6, u32 param_7, s32 param_8);

NetworkIcon *sub_0203A3B0(u32 param_1, enum HeapID param_2, u16 param_3, u16 param_4, u32 param_5, void *param_6, u32 param_7, s32 param_8) {
    NetworkIcon *icon;
    SysTask *pSVar1;

    sub_0203A4D4(param_7, param_5, param_8, param_2);
    sub_0203A59C(param_7, param_5, param_2);
    icon = Heap_AllocAtEnd(param_2, sizeof(NetworkIcon));
    pSVar1 = SysTask_CreateOnVWaitQueue(sub_0203A42C, icon, 5);
    icon->postVBlankTask = pSVar1;
    icon->x = param_3;
    icon->y = param_4;
    icon->unused = 0;
    icon->palette = (s8)((s32)(param_8 + ((u32)(param_8 >> 4) >> 0x1b)) >> 5);
    icon->vramType = (s8)param_7;
    icon->oamMode = 0;
    icon->strength = 3;
    icon->unused2 = param_6;
    icon->unused3 = 0;
    icon->isWifi = (s8)param_5;
    icon->createdOnSubScreen = 0;
    icon->screenId = 0;
    icon->oam = (GXOamAttr *)HW_OAM;
    return icon;
}
