#include "global.h"

#include "heap.h"
#include "overlay_07_battle_anim_funcs.h"
#include "sys_task.h"

typedef struct SpriteShakeInfo {
    s16 extentX;
    s16 extentY;
    s16 interval;
    s16 amount;
    int targets;
} SpriteShakeInfo;

// the layout of FlailContext (overlay_07_02226F04_02228F88_02228F88.c)
typedef struct UnkStruct_ov07_02228F14_ptr {
    BattleAnimScriptFuncCommon common;
    SpriteShakeInfo shakeInfo;
    struct Pokepic *battlerSprites[2];
    s64 unused;
    XYTransformContext shake;
} UnkStruct_ov07_02228F14_ptr;

s32 ov07_022227A8(void *, s16, s16, u32, s32);

s32 ov07_0222283C(void *, s32, s32, void *);

void ov07_02228F14(SysTask *arg0, void *param);

void ov07_02228F14(SysTask *arg0, void *param) {
    UnkStruct_ov07_02228F14_ptr *ptr = param;
    Point2D v0;
    u8 v1;

    v1 = ptr->common.state;
    switch (v1) {
    case 0:
        ov07_022227A8((void *)&ptr->shake, ptr->shakeInfo.extentX, ptr->shakeInfo.extentY, ptr->shakeInfo.interval, (s32) ptr->shakeInfo.amount);
        ptr->common.state = (u8)(ptr->common.state + 1);
        return;
    case 1:
        ov07_02231FA0(ptr->battlerSprites[1], &v0);
        if (ov07_0222283C((void *)&ptr->shake, v0.x, v0.y, ptr->battlerSprites[1]) == 0) {
            ptr->common.state = (u8)(ptr->common.state + 1);
            return;
        }
        return;
    default:
        ov07_0221C448(ptr->common.battleAnimSys, arg0);
        Heap_Free(ptr);
        break;
    }
}
