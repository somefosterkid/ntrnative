#include "global.h"

#include "error_handling.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sys_task.h"
#include "sys_task_api.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_anim.h"
#include "overlay_07_battle_bg_switch.h"

BattleBgSwitch *ov07_0221DF1C(BattleAnimSystem *system);

void ov07_0221ED14(SysTask *task, void *data);

extern s32 ov07_0223789C;

void ov07_0221ED94(BattleAnimSystem *system);

void ov07_0221ED94(BattleAnimSystem *system) {
    int v0;
    int v1;
    s16 type;
    s16 amount;
    BattleBgSwitch *bgSwitch;

    bgSwitch = ov07_0221DF1C(system);
    bgSwitch->fadeType = ov07_0221C4A8(system, 4);
    system->scriptPtr++;
    bgSwitch->bgID = *system->scriptPtr;
    system->scriptPtr++;
    v0 = *system->scriptPtr;
    system->scriptPtr++;
    bgSwitch->mode = (u16)v0;
    bgSwitch->flags = ((u32)(v0 & 0xFFFF0000)) >> 16;
    v1 = *system->scriptPtr;
    system->scriptPtr++;
    type = (s16)v1;
    amount = (s16)(((u32)(v1 & 0xFFFF0000)) >> 16);

    switch (type) {
    case 1:
        bgSwitch->unk_0E += amount;
        break;
    case 2:
        bgSwitch->unk_0E -= amount;
        break;
    case 3:
        bgSwitch->unk_0F += amount;
        break;
    case 4:
        bgSwitch->unk_0F -= amount;
        break;
    case 5:
        bgSwitch->unk_0E += amount;
        bgSwitch->unk_0F += amount;
        break;
    case 6:
        bgSwitch->unk_0E -= amount;
        bgSwitch->unk_0F -= amount;
        break;
    default: {
        void *sym = &ov07_0223789C;
        if (sym != 0L) {
            GF_AssertFail();
        }
    } break;
    }

    SysTask_CreateOnMainQueue(ov07_0221ED14, bgSwitch, 0x44CU);
}

typedef unsigned short undefined2;

typedef unsigned int undefined4;

void ov07_0221EE74(BattleAnimSystem *param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4);

void ov07_0221EE74(BattleAnimSystem *param_1, undefined4 param_2, undefined4 param_3, undefined4 param_4) {
    short sVar1;
    BattleBgAnim *iVar2;
    undefined4 *puVar3;
    undefined4 uVar4;
    puVar3 = param_1->scriptPtr;
    puVar3 = ((void *)puVar3) + 4;
    param_1->scriptPtr = puVar3;
    uVar4 = *param_1->scriptPtr;
    puVar3 = ((void *)puVar3) + 4;
    param_1->scriptPtr = puVar3;
    sVar1 = (short)*param_1->scriptPtr;
    puVar3 = ((void *)puVar3) + 4;
    param_1->scriptPtr = puVar3;
    iVar2 = ov07_0221DD8C(param_1->bgAnim, 1);
    switch (uVar4) {
    case 0:
        iVar2->stepX = sVar1;
        return;

    case 1:
        iVar2->stepY = sVar1;
        return;

    case 2:
        iVar2->offsetX = sVar1;
        return;

    case 3:
        iVar2->offsetX = sVar1;
    }

    return;
}
