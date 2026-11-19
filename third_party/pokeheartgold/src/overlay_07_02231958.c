#include "global.h"

#include "error_handling.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_anim_funcs.h"

struct BattleAnimSystem;

u8 ov07_0221FA04(BattleAnimSystem *system, int battler);

int ov07_02231958(BattleAnimSystem *system, int type);

s32 ov07_022319E0(u32 arg0);

int ov07_02231958(BattleAnimSystem *system, int type) {
    for (int battler = 0; battler < 4; battler++) {
        int battlerType = ov07_0221FA04(system, battler);
        if (battlerType == type) {
            return battler;
        }
    }

    return 0;
}

int ov07_0223197C(BattleAnimSystem *system, int battler);

int ov07_0223197C(BattleAnimSystem *system, int battler) {
    int type = ov07_0221FA04(system, battler);
    if (type == 0 || type == 1) {
        return type;
    }

    for (int otherBattler = 0; otherBattler < 4; otherBattler++) {
        int otherType = ov07_0221FA04(system, otherBattler);

        switch (type) {
        case 2:
            if (otherType == 4) {
                return otherBattler;
            }
            break;
        case 4:
            if (otherType == 2) {
                return otherBattler;
            }
            break;
        case 3:
            if (otherType == 5) {
                return otherBattler;
            }
            break;
        case 5:
            if (otherType == 3) {
                return otherBattler;
            }
            break;
        }
    }

    ((0) ? (void)0 : GF_AssertFail());

    return type;
}

s32 ov07_022319E0(u32 arg0) {
    switch (arg0) {
    case 0:
        return 1;
    case 1:
        return 0;
    case 2:
        return 3;
    case 3:
        return 2;
    case 4:
        return 5;
    case 5:
        return 4;
    default:
        GF_AssertFail();
        return 0;
    }
}
