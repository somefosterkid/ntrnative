#include "global.h"
#include "overlay_07_battle_anim_funcs.h"

s32 ov07_0221BFC0(BattleAnimSystem *system);

s32 ov07_0223192C(BattleAnimSystem *system, s32 battler);

s32 ov07_02231AD0(s32, u32 *, s32, s32);

u8 sub_02015530(void *);

void ov07_022208F8(void *arg0, BattleAnimSystem **arg1, void *arg2);

void ov07_022208F8(void *arg0, BattleAnimSystem **arg1, void *arg2) {
    BattleAnimSystem *v0;
    s32 v1;
    s32 v2;

    v0 = *arg1;
    v1 = sub_02015530(ov07_0221C478(v0));
    if (ov07_0223192C(v0, ov07_0221C470(v0)) != 3) {
        v2 = 1;
    } else {
        v2 = 0;
    }
    ov07_02231AD0(v2, arg2, ov07_0221BFC0(v0), v1);
}

void ov07_02220938(void *arg0, BattleAnimSystem **arg1, void *arg2);

void ov07_02220938(void *arg0, BattleAnimSystem **arg1, void *arg2) {
    BattleAnimSystem *v0;
    s32 v1;
    s32 v2;

    v0 = *arg1;
    v1 = sub_02015530(ov07_0221C478(v0));
    if (ov07_0223192C(v0, ov07_0221C468(v0)) != 3) {
        v2 = 1;
    } else {
        v2 = 0;
    }
    ov07_02231AD0(v2, arg2, ov07_0221BFC0(v0), v1);
}
