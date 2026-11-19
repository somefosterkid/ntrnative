#include "global.h"
#include "sys_task.h"
#include "overlay_07_battle_anim_system.h"

void ov07_02231514(u8 *, s16, s16);

void ov07_0223151C(u8 *, s16, s16);

void ov07_0223172C(void *, u8 *);

void ov07_0223174C(void *, u8 *);

void ov07_022318CC(void *param_1);

u8 ov07_0221FA04(BattleAnimSystem *arg0, s32 arg1);
u8 ov07_02231924(BattleAnimSystem *arg0, s32 arg1);

void ov07_022318CC(void *param_1) {
    u8 auStack_20[24];

    ov07_0223172C(param_1, auStack_20);
    ov07_02231514(auStack_20, 0, 3);
    ov07_0223151C(auStack_20, 0xc, 4);
    ov07_0223174C(param_1, auStack_20);
    return;
}

void ov07_022318F8(void *param_1);

void ov07_022318F8(void *param_1) {
    u8 auStack_20[24];

    ov07_0223172C(param_1, auStack_20);
    ov07_02231514(auStack_20, 0, 0xfffffffa);
    ov07_0223151C(auStack_20, 0xc, 4);
    ov07_0223174C(param_1, auStack_20);
    return;
}

u8 ov07_02231924(BattleAnimSystem *arg0, s32 arg1) {
    return ov07_0221FA04(arg0, arg1);
}
