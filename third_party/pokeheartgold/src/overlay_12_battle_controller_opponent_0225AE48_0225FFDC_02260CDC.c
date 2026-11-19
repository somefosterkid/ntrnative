#include "global.h"

#include "battle/battle.h"
#include "battle/battle_controller.h"

#include "error_handling.h"
#include "heap.h"
#include "sys_task_api.h"
#include "overlay_12_sprite_to_oamdata.h"

void *ov07_0221FB90(BattleSystem *, s32, int);

void ov12_02260CDC(SysTask *arg0, void *arg1);

void ov12_02260CDC(SysTask *task, void *ptr) {
    if (((SpriteToOAMData *)ptr)->state == 0) {
        if (((SpriteToOAMData *)ptr)->battlerData->btlMonObjData != NULL) {
            GF_AssertFail();
        }
        ((SpriteToOAMData *)ptr)->battlerData->btlMonObjData = ov07_0221FB90(*(BattleSystem **)ptr, 5, ((SpriteToOAMData *)ptr)->battler);
        ((SpriteToOAMData *)ptr)->state = (u8)(((SpriteToOAMData *)ptr)->state + 1);
        return;
    }
    BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32) ((SpriteToOAMData *)ptr)->battler, (s32) ((SpriteToOAMData *)ptr)->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}

s32 ov07_0221FE3C(void *, u8);

s32 ov07_0221FE70(void *);

void ov12_02260D28(SysTask *arg0, void *arg1);

void ov12_02260D28(SysTask *task, void *ptr) {
    u8 v0;

    if (((SpriteToOAMData *)ptr)->battlerData->btlMonObjData == NULL) {
        GF_AssertFail();
    }
    if (ov07_0221FE70(((SpriteToOAMData *)ptr)->battlerData->btlMonObjData) == 3) {
        v0 = ((SpriteToOAMData *)ptr)->state;
        if ((u32)v0 < 5U) {
            ((SpriteToOAMData *)ptr)->state = (u8)(v0 + 1);
            return;
        }
    }
    ov07_0221FE3C(((SpriteToOAMData *)ptr)->battlerData->btlMonObjData, ((SpriteToOAMData *)ptr)->battler);
    ((SpriteToOAMData *)ptr)->battlerData->btlMonObjData = NULL;
    BattleController_EmitClearCommand(*(BattleSystem **)ptr, (s32) ((SpriteToOAMData *)ptr)->battler, (s32) ((SpriteToOAMData *)ptr)->command);
    Heap_Free(ptr);
    SysTask_Destroy(task);
}
