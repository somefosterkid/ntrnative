#include "global.h"
#include "overlay_07_battle_anim_system.h"

struct BattleAnimSystem;

u8 ov07_02231924(BattleAnimSystem *system, int battler);

s8 ov07_0221FF34(BattleAnimSystem *system, int startBattler, int endBattler);

void ov07_0221FF74(void);

s8 ov07_0221FF34(BattleAnimSystem *system, int startBattler, int endBattler) {
    s8 sign = +1;
    int startType = ov07_02231924(system, startBattler);
    int endType = ov07_02231924(system, endBattler);

    switch (startType) {
    case 0:
    default:

        break;
    case 1:
        sign = -1;
        break;
    case 2:

        break;
    case 3:
        sign = -1;
        break;
    case 4:

        break;
    case 5:
        sign = -1;
        break;
    }

    return sign;
}

void ov07_0221FF74(void) {
}
