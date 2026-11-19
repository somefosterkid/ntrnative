#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "particle_system.h"

s32 ov07_0221BFC0(BattleAnimSystem *);

u8 ov07_02231924(BattleAnimSystem *, s32);

s32 ov07_02231B10(s32, u32 *, s32, s32);

u8 sub_02015530(ParticleSystem *);

void ov07_02231C50(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231DD0(VecFx32 *pos);

void ov07_02231C50(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231B10(v0, arg2, ov07_0221BFC0(arg0), v1);
}

s32 ov07_02231B20(s32, u32 *, s32, s32);

void ov07_02231C80(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231C80(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231B20(v0, arg2, ov07_0221BFC0(arg0), v1);
}

s32 ov07_02231B30(s32, u32 *, s32, s32);

void ov07_02231CB0(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231CB0(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231B30(v0, arg2, ov07_0221BFC0(arg0), v1);
}

s32 ov07_02231B40(s32, u32 *, s32, s32);

void ov07_02231CE0(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231CE0(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231B40(v0, arg2, ov07_0221BFC0(arg0), v1);
}

s32 ov07_02231B50(s32, u32 *, s32, s32);

void ov07_02231D10(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231D10(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231B50(v0, arg2, ov07_0221BFC0(arg0), v1);
}

s32 ov07_02231B60(s32, u32 *, s32, s32);

void ov07_02231D40(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231D40(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231B60(v0, arg2, ov07_0221BFC0(arg0), v1);
}

s32 ov07_02231B70(s32, u32 *, s32, s32);

void ov07_02231D70(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231D70(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231B70(v0, arg2, ov07_0221BFC0(arg0), v1);
}

s32 ov07_02231B80(s32, u32 *, s32, s32);

void ov07_02231DA0(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231DA0(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231B80(v0, arg2, ov07_0221BFC0(arg0), v1);
}

void ov07_02231DD0(VecFx32 *pos) {
    pos->x = 0;
    pos->y = 16512 + ((60) * 172);
    pos->z = 0;
}
