#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"
#include "particle_system.h"

s32 ov07_0221BFC0(BattleAnimSystem *);

u8 ov07_02231924(BattleAnimSystem *, s32);

s32 ov07_02231AD0(s32, u32 *, s32, s32);

u8 sub_02015530(ParticleSystem *);

void ov07_02231B90(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231B90(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231AD0(v0, arg2, ov07_0221BFC0(arg0), v1);
}

s32 ov07_02231AE0(s32, u32 *, s32, s32);

void ov07_02231BC0(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231BC0(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231AE0(v0, arg2, ov07_0221BFC0(arg0), v1);
}

s32 ov07_02231AF0(s32, u32 *, s32, s32);

void ov07_02231BF0(BattleAnimSystem *arg0, s32 arg1, void *arg2);

void ov07_02231BF0(BattleAnimSystem *arg0, s32 arg1, void *arg2) {
    s32 v0;
    s32 v1;

    v1 = sub_02015530(ov07_0221C478(arg0));
    v0 = ov07_02231924(arg0, arg1);
    ov07_02231AF0(v0, arg2, ov07_0221BFC0(arg0), v1);
}
