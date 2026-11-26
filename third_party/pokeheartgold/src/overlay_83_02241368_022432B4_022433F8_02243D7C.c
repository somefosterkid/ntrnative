#include "global.h"

#include "unk_020379A0.h"
#include "overlay_83_battle_castle_opponent_app.h"

s32 ov83_02245094(Window *window);

s32 ov83_022450A8(void *, u16, u16);

s32 ov83_02243D7C(BattleCastleOpponentApp *arg0);

s32 ov83_02243D7C(BattleCastleOpponentApp *arg0) {
    u8 v0;
    u8 v1;

    v1 = arg0->subState;
    switch (v1) {
    case 0:
        if (ov83_022450A8(arg0, 0x17, 0) == 1) {
            arg0->syncDelay = 0x1EU;
            arg0->subState = (u8)(arg0->subState + 1);
        }
    default:
        break;
    case 1:
        v0 = arg0->syncDelay;
        if (v0 != 0) {
            arg0->syncDelay = (u8)(v0 - 1);
        }
        if (arg0->syncDelay == 0) {
            sub_02037BEC();
            sub_02037AC0(0x86U);
            arg0->subState = (u8)(arg0->subState + 1);
        }
        break;
    case 2:
        if (sub_02037B38(0x86U) == 1) {
            sub_02037BEC();
            ov83_02245094(&arg0->windows[7]);
            return 1;
        }
        break;
    }
    return 0;
}
