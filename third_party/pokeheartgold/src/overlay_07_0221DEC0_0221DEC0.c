#include "global.h"
#include "overlay_07_battle_anim_system.h"
#include "overlay_07_battle_bg_switch.h"

struct BattleAnimSystem;

struct BattleBgSwitch;

BOOL ov07_0221DEB0(int flags, int flag);

BOOL BattleBgSwitch_AnimNone(BattleBgSwitch *bgSwitch);

BOOL ov07_0221E6D4(BattleBgSwitch *bgSwitch);

BOOL ov07_0221ECE8(BattleBgSwitch *bgSwitch);

BOOL BattleBgSwitch_AnimCancel(BattleBgSwitch *bgSwitch);

BOOL ov07_0221E788(BattleBgSwitch *bgSwitch);

BOOL ov07_0221E868(BattleBgSwitch *bgSwitch);

extern BOOL (*const ov07_02234C40[7])(BattleBgSwitch *);

void ov07_0221DEC0(BattleBgSwitch *bgSwitch);

typedef struct {
    const u32 a[6];
} ov07_02234BA8_Array;

extern const ov07_02234BA8_Array ov07_02234BA8;

void ov07_0221DEC0(BattleBgSwitch *bgSwitch) {
    ov07_02234BA8_Array flags = ov07_02234BA8;

    for (int i = 0; i < (sizeof(flags.a) / sizeof(*(flags.a))); i++) {
        if (!ov07_0221DEB0(bgSwitch->flags, flags.a[i])) {
            continue;
        }

        int id = 0;
        int flag = flags.a[i] >> 16;

        while (flag >= 2) {
            flag /= 2;
            id++;
        }

        ov07_02234C40[id](bgSwitch);
    }
}
