#include "global.h"

#include "battle/battle_system.h"

#include "overlay_12_battle_controller_opponent.h"
#include "pokemon.h"
#include "pokepic.h"

void ov12_02261F38(BattleSystem *battleSystem, s32 arg1, u32 a3, Pokepic *pokepic, NARC *narc, s32 arg5, s32 r4, s32 a4, u32 r6) {
    u8 v0;
    s32 v1;

    v0 = 0;
    if (BattleSystem_AreBattleAnimationsOn(battleSystem) == 1) {
        Pokepic_StartAnim(pokepic, 1);
        sub_0207294C(narc, ov12_0223B750(battleSystem), pokepic, (u16)arg5, a4, 0, arg1);
        sub_020729A4(narc, &v0, (u16)arg5, a3);
    }
    if (a4 == 2) {
        v1 = 0x75;
    } else {
        v1 = -0x75;
    }
    if (v0 == 0) {
        v0 = 8;
    }
    sub_0207204C(BattleSystem_GetChatotVoice(battleSystem, arg1), r6, (u16)arg5, r4, v1, 0x7FU, 0U, 5U, (u8)(s32)v0);
}
