#include "global.h"
#include "overlay_07_battle_anim_funcs.h"
#include "overlay_07_battle_anim_system.h"

BOOL ov07_0221BFC0(BattleAnimSystem *system);

u32 ov07_0223192C(BattleAnimSystem *system, int battler);

fx32 ov07_02222040(fx32 start, fx32 end, u32 steps);

s32 ov07_02222004(BattleAnimSystem *system, int battler) {
    s32 v0;
    s32 v1;

    v1 = 1;
    v0 = ov07_0223192C(system, battler);
    if (ov07_0221BFC0(system) != 0) {
        if (v0 == 3) {
            v1 = -1;
        }
    } else if (v0 == 4) {
        v1 = -1;
    }
    return v1;
}

s32 ov07_0222202C(BattleAnimSystem *system, int battler) {
    s32 v0;

    v0 = 1;
    if (ov07_0223192C(system, battler) == 4) {
        v0 = -1;
    }
    return v0;
}

fx32 ov07_02222040(fx32 start, fx32 end, u32 steps) {
    return FX_Div(end - start, steps << 12);
}
