#include "global.h"
struct BattleCastleOpponentApp;

#include "unk_02035900.h"
#include "overlay_83_battle_castle_opponent_app.h"

void ov83_02245254(u32 param_1, u32 param_2, u16 *param_3, u8 *param_4);

void ov83_02245254(u32 param_1, u32 param_2, u16 *param_3, u8 *param_4) {
    u16 uVar1;

    uVar1 = sub_0203769C();
    if (param_1 != uVar1) {
        *(s8 *)(param_4 + 0x5b6) = (s8)*param_3;
    }
    return;
}

u8 ov83_02247768(u8);

void ov83_02245270(struct BattleCastleOpponentApp *param_1);

void ov83_02245270(BattleCastleOpponentApp *param_1) {
    s32 iVar1;

    iVar1 = ov83_02247768(param_1->numSlots);
    *(u8 *)(param_1->statsUnlocked + iVar1) = 1;
    return;
}
