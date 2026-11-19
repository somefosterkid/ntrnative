#include "global.h"

#include "constants/heap.h"

#include "screen_fade.h"
#include "unk_020379A0.h"
#include "overlay_82.h"
#include "overlay_82_battle_hall_app.h"

void ov82_0223E070(u8 *arg0);

s32 ov80_0223792C(u8);

s32 ov82_0223E0B0(void *);

s32 ov82_0223DFBC(void *arg0);

s32 ov82_0223EFCC(void *, void *, s32, s32, u8, u8, u8, u8);
s32 ov82_0223F040(u8 *, u8 *, u8, u8, u8, u8);
s32 ov82_0223F134(u8 *, u8 *);

s32 ov82_0223DFBC(void *arg0) {
    u8 v0;

    v0 = ((BattleHallApp *)arg0)->subState;
    switch (v0) {
    case 0:
        if ((((BattleHallApp *)arg0)->isSynced == 0) && (ov80_0223792C(((BattleHallApp *)arg0)->challengeType) == 1)) {
            sub_02037BEC();
            sub_02037AC0(0x70U);
        }
        ((BattleHallApp *)arg0)->subState = (u8)(((BattleHallApp *)arg0)->subState + 1);
    default:
        break;
    case 1:
        if ((((BattleHallApp *)arg0)->isSynced == 0) && (ov80_0223792C(((BattleHallApp *)arg0)->challengeType) == 1)) {
            if (sub_02037B38(0x70U) == 1) {
                sub_02037BEC();
                ((BattleHallApp *)arg0)->isSynced = 1U;
                ((BattleHallApp *)arg0)->subState = (u8)(((BattleHallApp *)arg0)->subState + 1);
            }
        } else {
            ((BattleHallApp *)arg0)->subState = (u8)(((BattleHallApp *)arg0)->subState + 1);
        }
        break;
    case 2:
        ov82_0223E070((u8 *)arg0);
        ov82_0223E0B0(arg0);
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, 0U, 6, 3, HEAP_ID_105);
        ((BattleHallApp *)arg0)->subState = (u8)(((BattleHallApp *)arg0)->subState + 1);
        break;
    case 3:
        if (IsPaletteFadeFinished() == 1) {
            return 1;
        }
        break;
    }
    return 0;
}

void ov82_0223E070(u8 *arg0) {
    ov82_0223F040(arg0, arg0 + 0x6C, 1, 2, 0, 0);
    ov82_0223F134(arg0, arg0 + 0x6C);
    ov82_0223EFCC(arg0, arg0 + 0x5C, 0, 0, 1, 2, 0, 0);
}
