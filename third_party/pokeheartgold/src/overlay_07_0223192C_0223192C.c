#include "global.h"
#include "overlay_07_battle_anim_system.h"

u8 ov07_0221FA04(BattleAnimSystem *system, int battler);

u32 ov07_0223192C(BattleAnimSystem *system, int battler);

u32 ov07_0223192C(BattleAnimSystem *system, int battler) {
    u32 uVar1;
#if defined(SDK_PORT) || defined(NONMATCHING)
    u32 unaff_r4 = 3; // the DS leaves the register unset for a battler type past 5
#else
    u32 unaff_r4;
#endif

    uVar1 = ov07_0221FA04(system, battler);
    switch (uVar1) {
    case 0:
    case 2:
    case 4:
        unaff_r4 = 3;
        break;
    case 1:
    case 3:
    case 5:
        unaff_r4 = 4;
    }
    return unaff_r4;
}
