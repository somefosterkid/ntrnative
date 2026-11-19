#include "global.h"

#include "unk_020379A0.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_02245094(void *);

s32 ov83_022450A8(void *, u16, u16);

s32 ov83_02243D7C(void *arg0);

s32 ov83_02243D7C(void *arg0) {
    u8 v0;
    u8 v1;

    v1 = ((BattleCastleOpponentApp *)arg0)->subState;
    switch (v1) {
    case 0:
        if (ov83_022450A8(arg0, 0x17, 0) == 1) {
            ((BattleCastleOpponentApp *)arg0)->syncDelay = 0x1EU;
            ((BattleCastleOpponentApp *)arg0)->subState = (u8)(((BattleCastleOpponentApp *)arg0)->subState + 1);
        }
    default:
        break;
    case 1:
        v0 = ((BattleCastleOpponentApp *)arg0)->syncDelay;
        if (v0 != 0) {
            ((BattleCastleOpponentApp *)arg0)->syncDelay = (u8)(v0 - 1);
        }
        if (((BattleCastleOpponentApp *)arg0)->syncDelay == 0) {
            sub_02037BEC();
            sub_02037AC0(0x86U);
            ((BattleCastleOpponentApp *)arg0)->subState = (u8)(((BattleCastleOpponentApp *)arg0)->subState + 1);
        }
        break;
    case 2:
        if (sub_02037B38(0x86U) == 1) {
            sub_02037BEC();
            ov83_02245094(arg0 + 0xC0);
            return 1;
        }
        break;
    }
    return 0;
}
